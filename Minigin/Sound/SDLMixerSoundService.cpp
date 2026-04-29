#include "SDLMixerSoundService.h"
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

struct SoundEvent
{
    std::string filename;
    int volume;
};

class dae::SDLMixerSoundService::Impl
{
public:
    Impl()
    {
        if (!MIX_Init())
        {
            SDL_Log("[Sound] MIX_Init failed: %s", SDL_GetError());
            return;
        }

        m_pMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
        if (!m_pMixer)
        {
            SDL_Log("[Sound] MIX_CreateMixerDevice failed: %s", SDL_GetError());
            MIX_Quit();
            return;
        }

        m_Running = true;
        m_Thread = std::thread(&Impl::ProcessQueue, this);
    }

    ~Impl()
    {
        {
            std::lock_guard lock(m_Mutex);
            m_Running = false;
        }
        m_CV.notify_all();
        if (m_Thread.joinable())
            m_Thread.join();

        for (auto* pTrack : m_ActiveTracks)
        {
            if (pTrack)
            {
                MIX_StopTrack(pTrack, 0);
                MIX_DestroyTrack(pTrack);
            }
        }
        m_ActiveTracks.clear();

        for (auto& [name, pAudio] : m_Cache)
        {
            if (pAudio)
                MIX_DestroyAudio(pAudio);
        }
        m_Cache.clear();

        if (m_pMixer)
        {
            MIX_DestroyMixer(m_pMixer);
            m_pMixer = nullptr;
        }

        MIX_Quit();
    }

    void Enqueue(const std::string& filename, int volume)
    {
        {
            std::lock_guard lock(m_Mutex);
            m_Queue.push({ filename, volume });
        }
        m_CV.notify_one();
    }

    void SetMasterVolume(int /*volume*/) {}

private:
    void CleanupFinishedTracks()
    {
        m_ActiveTracks.erase(
            std::remove_if(m_ActiveTracks.begin(), m_ActiveTracks.end(), [](MIX_Track* pTrack)
                {
                    if (!pTrack) return true;
                    if (MIX_TrackPlaying(pTrack)) return false;
                    MIX_DestroyTrack(pTrack);
                    return true;
                }),
            m_ActiveTracks.end()
        );
    }

    void ProcessQueue()
    {
        while (true)
        {
            std::unique_lock lock(m_Mutex);
            m_CV.wait(lock, [this] { return !m_Queue.empty() || !m_Running; });

            if (!m_Running && m_Queue.empty())
                return;

            SoundEvent ev = m_Queue.front();
            m_Queue.pop();
            lock.unlock();

            if (!m_pMixer) continue;

            CleanupFinishedTracks();

            if (m_Cache.find(ev.filename) == m_Cache.end())
            {
                MIX_Audio* pAudio = MIX_LoadAudio(m_pMixer, ev.filename.c_str(), true);
                if (!pAudio)
                {
                    SDL_Log("[Sound] MIX_LoadAudio failed for '%s': %s",
                        ev.filename.c_str(), SDL_GetError());
                    continue;
                }
                m_Cache[ev.filename] = pAudio;
            }

            MIX_Audio* pAudio = m_Cache[ev.filename];

            MIX_Track* pTrack = MIX_CreateTrack(m_pMixer);
            if (!pTrack)
            {
                SDL_Log("[Sound] MIX_CreateTrack failed: %s", SDL_GetError());
                continue;
            }

            if (!MIX_SetTrackAudio(pTrack, pAudio))
            {
                SDL_Log("[Sound] MIX_SetTrackAudio failed: %s", SDL_GetError());
                MIX_DestroyTrack(pTrack);
                continue;
            }

            if (!MIX_PlayTrack(pTrack, 0))
            {
                SDL_Log("[Sound] MIX_PlayTrack failed: %s", SDL_GetError());
                MIX_DestroyTrack(pTrack);
                continue;
            }

            m_ActiveTracks.push_back(pTrack);
        }
    }

    std::queue<SoundEvent>                      m_Queue;
    std::unordered_map<std::string, MIX_Audio*> m_Cache;
    std::vector<MIX_Track*>                     m_ActiveTracks;
    std::thread                                 m_Thread;
    std::mutex                                  m_Mutex;
    std::condition_variable                     m_CV;
    bool                                        m_Running{ false };
    MIX_Mixer* m_pMixer{ nullptr };
};

dae::SDLMixerSoundService::SDLMixerSoundService()
    : m_pImpl(std::make_unique<Impl>()) {}

dae::SDLMixerSoundService::~SDLMixerSoundService() = default;

void dae::SDLMixerSoundService::Play(const std::string& filename, int volume)
{
    m_pImpl->Enqueue(filename, volume);
}

void dae::SDLMixerSoundService::SetMasterVolume(int volume)
{
    m_pImpl->SetMasterVolume(volume);
}