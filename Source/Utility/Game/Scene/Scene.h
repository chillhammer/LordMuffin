#pragma once
#include <Game/GameManager.h>
#include <StateMachine/State.h>
#include <StateMachine/StateMachine.h>
#include <GameObject/GameObject.h>

namespace Skel
{
	// Collection of entities that encompasses what is shown on screen
	class Scene
	{
	public:
		Scene() {};

	private:
		std::vector<GameObject> m_Objects; // TODO: Game object templates

		// TODO: Scene serialization
	};
}