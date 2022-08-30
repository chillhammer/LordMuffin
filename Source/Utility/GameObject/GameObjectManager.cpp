#include "SkelPCH.h"
#include "GameObjectManager.h"
#include "EventSystem/Events/GameObjectEvent.h"
#include <Resources/ResourceManager.h>

namespace Skel
{
	GameObjectManager& Skel::GameObjectManager::Instance()
	{
		static GameObjectManager instance;
		return instance;
	}
	void GameObjectManager::Init()
	{
		//m_GameObjects.reserve(100);
	}
	void GameObjectManager::LoadScene(const std::string& scene)
	{
		for (GameObject* obj : m_GameObjects)
		{
			delete obj;
		}
		m_GameObjects.clear();
		auto& objTempls = Resources.GetScene(scene)->GetObjectTemplates();
		for (auto objTemplPtr : objTempls)
		{
			m_GameObjects.emplace_back(objTemplPtr->Instantiate());
		}
		for (auto obj : m_GameObjects)
		{
			obj->OnSceneCreatedComponents();
		}
		SceneLoadedEvent e(m_GameObjects);
		SceneLoaded.Notify(e);

	}
	GameObject* GameObjectManager::InstantiateObject(GameObjectTemplatePtr obj)
	{
		GameObject* objInst = obj->Instantiate();
		m_GameObjects.emplace_back(objInst);
		return objInst;
	}
	void GameObjectManager::DestroyObject(GameObject* obj)
	{
		m_GameObjectsDestroyQueue.emplace(std::move(obj));
	}
	GameObject* GameObjectManager::FindObjectByName(const std::string& name)
	{
		for (GameObject* obj : m_GameObjects)
		{
			if (obj->GetName() == name)
			{
				return obj;
			}
		}
		return nullptr;
	}

	GameObjectManager::GameObjectManager()
	{
	}

	// Runs all update, draw, destructions for all objects
	void GameObjectManager::Run()
	{
		for(size_t i = 0; i < m_GameObjects.size(); ++i)
		{
			GameObject* const o = m_GameObjects.at(i);
			o->UpdateComponents();
		}
		
		for (size_t i = 0; i < m_GameObjects.size(); ++i)
		{
			GameObject* const o = m_GameObjects.at(i);
			o->PostUpdateComponents();
		}

		std::vector<rttr::type> updatedRTTRTypes;
		for (size_t i = 0; i < m_GameObjects.size(); ++i)
		{
			GameObject* const o = m_GameObjects.at(i);
			o->StaticUpdateComponents(updatedRTTRTypes);
		}

#ifndef  SERVER
		for (size_t i = 0; i < m_GameObjects.size(); ++i)
		{
			GameObject* const o = m_GameObjects.at(i);
			o->DrawComponents();
		}
#endif // ! SERVER

		while (m_GameObjectsDestroyQueue.size() > 0)
		{
			GameObject* objToDestroy = m_GameObjectsDestroyQueue.front();
			m_GameObjectsDestroyQueue.pop();
			m_GameObjects.erase(std::remove(m_GameObjects.begin(), m_GameObjects.end(), objToDestroy), m_GameObjects.end());
			delete objToDestroy;
		}
	}
}
