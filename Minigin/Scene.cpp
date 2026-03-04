#include <algorithm>
#include "Scene.h"
#include <cassert>

using namespace dae;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove()
{
	m_objects.erase( std::remove_if(
						m_objects.begin(),
						m_objects.end(),
						[](const auto& obj){ return obj->GetMarkedForDeletion(); }),
					m_objects.end() );
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update(float deltaTime)
{
	for(auto& object : m_objects)
	{
		if (!object->GetParent()) // Only update root objects, as children will be updated by their parents
			object->Update(deltaTime);
	}

	Remove(); //Remove objects that were marked for deletion during the update
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		if (!object->GetParent()) // Only update root objects, as children will be updated by their parents
			object->Render();
	}
}

