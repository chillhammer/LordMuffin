#pragma once
#include <string>
#include <rttr/registration>
#include <rttr/type>
#include <rttr/registration_friend.h>

namespace Skel
{
	class GameObjectComponent
	{
	public:

		GameObjectComponent() : m_Owner(nullptr) { };

		// Call when all components are created for object
		virtual void OnCreated() {};

		virtual void Update() {};
		virtual void Draw() {};


		void SetOwner(class GameObject* obj) { m_Owner = obj; }
		class GameObject* GetOwner() { return m_Owner; }

		virtual ~GameObjectComponent() {};

	protected:
		class GameObject* m_Owner;
		RTTR_ENABLE()	
	};
	typedef std::shared_ptr<GameObjectComponent> ComponentPtr;
}