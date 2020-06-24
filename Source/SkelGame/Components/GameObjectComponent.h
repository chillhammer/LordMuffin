#pragma once
#include <string>

namespace Skel
{
	class GameObjectComponent
	{
	public:
		typedef std::string ComponentIDType;

		GameObjectComponent() : m_Owner(nullptr) {};

		virtual const ComponentIDType& GetType() = 0;
		virtual const ComponentIDType& GetFamilyID() = 0;

		virtual void Update() {};

		void SetOwner(class GameObject* obj) { m_Owner = obj; }
		class GameObject* GetOwner() { return m_Owner; }

	private:
		class GameObject* m_Owner;
	};
}