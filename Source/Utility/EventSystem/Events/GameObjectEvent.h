#pragma once

#include <EventSystem/Event.h>


namespace Skel
{
	/**
		A Key Event is related to game object and game object manager
	**/
	EVENT_CLASS_SIMPLE(SceneLoaded,
		SceneLoadedEvent(std::vector<class GameObject*>& GameObjects) : GameObjects(GameObjects) {};
		std::vector<class GameObject*>& GameObjects;
	);
}