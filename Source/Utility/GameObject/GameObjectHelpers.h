#pragma once
#include <Game/GameManager.h>

namespace Skel::Objects
{
	template<typename T>
	T& FindFirstComponent()
	{
		const std::vector<GameObject*>& objects = Game.Objects();
		for (GameObject* obj : objects)
		{
			if (obj->HasComponent<T>())
			{
				return obj->GetComponent<T>();
			}
		}
		ASSERT(false, "Cannot find component. Does not exist");
		return T();
	}

	template<typename T>
	ComponentPtr FindFirstComponentPtr()
	{
		const std::vector<GameObject*>& objects = Game.Objects();
		for (GameObject* obj : objects)
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
		const std::vector<GameObject*>& objects = Game.Objects();
		for (GameObject* obj : objects)
		{
			if (obj->HasComponent<T>())
			{
				return true;
			}
		}
		return false;
	}
}