#pragma once
#include <EventSystem/IObserver.h>
#include "GameObjectComponent.h"

#define Components ComponentManager::Instance()
namespace Skel
{
	/**
		Manager singleton object handling component creation
	**/
	class ComponentManager : IObserver
	{
	public:
		static ComponentManager& Instance();

		void OnEvent(const Subject* subject, Event& event);
		~ComponentManager() {};
	private:
		ComponentManager() {};
	};
}