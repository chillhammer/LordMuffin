#pragma once
#include <Transform/Transform.h>
#include <Graphics/Model/Model.h>
#include <Components/GameObjectComponent.h>
#include "GameObjectHelpers.h"
#include "BoundingBox.h"


namespace Skel
{
	/**
		Base class for all game objects in the game.
		Each has a unique identifier to check if they are the same
	**/
	class GameObject
	{
		friend class GameObjectTemplate;
	public:
		GameObject();
		GameObject(const GameObject& other);
		int GetID() const;
		void AttachToParent(const GameObject* parent);


		// Add new component
		template<typename T, typename... TArgs>
		T& AddComponent(TArgs&& ... args)
		{
			// Assert has component
			ComponentPtr componentPtr(new T(std::forward<TArgs>(args)...));
			AddComponent(componentPtr);
		}
		// Add existing component
		void AddComponent(ComponentPtr);

		// Gets component
		template<typename T>
		T& GetComponent() const
		{
			uint64 componentID = rttr::type::get<T>().get_id();
			auto componentItr = m_ComponentMap.find(componentID);
			ASSERT(componentItr != m_ComponentMap.end(), "GetComponent Failed: component does not exist");
			return *static_cast<T*>(&(*componentItr->second));
		}
		// Gets component ptr
		template<typename T>
		ComponentPtr GetComponentPtr() const
		{
			uint64 componentID = rttr::type::get<T>().get_id();
			auto componentItr = m_ComponentMap.find(componentID);
			ASSERT(componentItr != m_ComponentMap.end(), "GetComponent Failed: component does not exist");
			if (componentItr != m_ComponentMap.end())
			{
				return *componentItr;
			}
			return nullptr;
		}
		// Checks if has component
		template<typename T>
		bool HasComponent() const
		{
			uint64 componentID = rttr::type::get<T>().get_id();
			auto componentItr = m_ComponentMap.find(componentID); // REDO Component map. uint64 doesnt work
			return componentItr != m_ComponentMap.end();
		}

		void SetName(const std::string& name) { m_Name = name; }
		const std::string& GetName() { return m_Name; }

		void OnSceneCreatedComponents();
		void UpdateComponents();
		void PostUpdateComponents();
		void DrawComponents();

		void Destroy();
		virtual ~GameObject();
	public:
		Transform ObjectTransform;
	private:
		static int GetNextID();
		std::vector<ComponentPtr> m_Components;
		std::map<uint64, ComponentPtr> m_ComponentMap; // Using type id as key
		int m_ID;
		std::string m_Name;
	};
}