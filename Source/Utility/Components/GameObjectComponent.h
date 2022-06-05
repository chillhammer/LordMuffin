#pragma once

#include <rttr/rttr_enable.h>
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
		// Call when created from Load Scene and all objects are loaded in
		virtual void OnSceneCreated() {};

		virtual void Update() {};
		virtual void PostUpdate() {};
		virtual void Draw() {};


		void SetOwner(class GameObject* obj) { m_Owner = obj; }
		class GameObject* GetOwner() const { return m_Owner; }
		
		void SetAlive(bool alive) { m_Alive = alive; }
		bool GetAlive() const { return m_Alive; }

		virtual ~GameObjectComponent() {};

	protected:
		class GameObject* m_Owner;
		bool m_Alive;
		RTTR_ENABLE()	
	};
	typedef std::shared_ptr<GameObjectComponent> ComponentPtr;
}