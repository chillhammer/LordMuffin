#pragma once
#include <Game/Scene/Scene.h>
#include <Server/ServerManager.h>

#define Objects GameObjectManager::Instance()
namespace Skel
{
	/**
		Manager singleton object handling all game objects
		Can be used across client and server
	**/
	class GameObjectManager
	{
		friend class Skel::Net::ServerManager;
		friend class GameManager;
	public:
		static GameObjectManager& Instance();
		void Init();
		void LoadScene(const std::string& scene);
		GameObject* InstantiateObject(GameObjectTemplatePtr);
		void DestroyObject(class GameObject* obj);

		#pragma region Object Helpers
		template<typename T>
		T& FindFirstComponent()
		{
			for (GameObject* obj : m_GameObjects)
			{
				if (obj->HasComponent<T>())
				{
					return obj->GetComponent<T>();
				}
			}
			ASSERT(false, "Cannot find component. Does not exist");
			return std::move(T());
		}

		template<typename T>
		ComponentPtr FindFirstComponentPtr()
		{
			for (GameObject* obj : m_GameObjects)
			{
				if (obj->HasComponent<T>())
				{
					return obj->GetComponentPtr<T>();
				}
			}
			return nullptr;
		}

		template<typename T>
		bool ComponentExists()
		{
			for (GameObject* obj : m_GameObjects)
			{
				if (obj->HasComponent<T>())
				{
					return true;
				}
			}
			return false;
		}
		GameObject* FindObjectByName(const std::string& name);
		#pragma endregion

		~GameObjectManager() {};
	private:
		GameObjectManager();

		void Run();

		std::vector<GameObject*> m_GameObjects;
		std::queue<GameObject*> m_GameObjectsDestroyQueue;
	};
}