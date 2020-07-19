#include "SkelPCH.h"
#include "GameObjectHelpers.h"

namespace Skel
{
	GameObject* Skel::Objects::FindObjectByName(const std::string& name)
	{
		const std::vector<GameObject*>& objects = Game.Objects();
		for (GameObject* obj : objects)
		{
			if (obj->GetName() == name)
			{
				return obj;
			}
		}
		return nullptr;
	}
}