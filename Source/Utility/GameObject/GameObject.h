#pragma once
#include <Transform/Transform.h>
#include <Graphics/Model/Model.h>
#include <Components/GameObjectComponent.h>
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
		GameObject(std::string modelName);
		GameObject(std::string modelName, Vector3 position);
		GameObject(const GameObject& other);
		int GetID() const;
		void AttachToParent(const GameObject* parent);


		template<typename T, typename... TArgs>
		T& AddComponent(TArgs&& ... args)
		{
			// Assert has component
			T* componentPtr(new T(std::forward<TArgs>(args)...));
			componentPtr->SetOwner(this);
			ComponentPtr ptr(componentPtr);
			m_Components.emplace_back(std::move(ptr));

			// Add to array using type as index
			// Set bitfield index to true
		}
		void AddComponent(ComponentPtr);

		template<typename T>
		T& GetComponent()
		{
			for (auto& component : m_Components)
			{
				rttr::type componentType = rttr::type::get(*component);
				if (componentType.is_derived_from<T>() || componentType == rttr::type::get<T>())
				{
					return *static_cast<T*>(&(*component));
				}
			}
			ASSERT(false, "GetComponent Failed: component does not exist");
			return T();
		}

		bool IsColliding(const GameObject& other) const;
		bool IsCollidingAtPosition(const GameObject& other, Vector3 newPos) const;
		void DrawComponents();

		void Draw();
		void Draw(const ShaderPtr& shader);
		void DrawBoundingBox() const;
		virtual void PreDraw() {}
	public:
		Transform ObjectTransform;
		void SetBoundingBox(Vector3 center, Vector3 halfExtents);
		BoundingBox GetBoundingBox() const;
	protected:
		ModelPtr m_Model;
		BoundingBox m_BoundingBox;
	private:
		static int GetNextID();
		std::vector<ComponentPtr> m_Components;
		int m_ID;
	};

	typedef std::shared_ptr<GameObject> GameObjectPtr;
}