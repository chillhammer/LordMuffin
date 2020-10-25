#include "SkelPCH.h"
#include "ComponentManager.h"

namespace Skel
{
	ComponentManager& ComponentManager::Instance()
	{
		static ComponentManager instance;
		return instance;
	}

	void ComponentManager::OnEvent(const Subject* subject, Event& event)
	{
	}

}
