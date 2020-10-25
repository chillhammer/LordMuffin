#include <skelpch.h>
#include <Resources/ResourceManager.h>
#include <GameObject/GameObjectManager.h>
#include "GameObject.h"

namespace Skel
{
	GameObject::GameObject()
	{
		m_ID = GetNextID();
	}
	GameObject::GameObject(const GameObject & other)
	{
		ASSERT(false, "Cannot copy construct game object");
	}
	// Returns unique identifier for this object
	int GameObject::GetID() const
	{
		return m_ID;
	}
	// Sets transform relative to parameter's transform
	// Updates Position to be local to new parent
	// Can set to nullptr, to have no parent
	void GameObject::AttachToParent(const GameObject* parent)
	{
		if (ObjectTransform.Parent == &(parent->ObjectTransform))
			return;
		// Detach from current parent
		if (ObjectTransform.Parent)
		{
			ObjectTransform.Position += ObjectTransform.Parent->GetGlobalPosition();
			ObjectTransform.Rotation += ObjectTransform.Parent->GetGlobalRotation();
			ObjectTransform.Parent = nullptr;
		}
		// Update to new parent
		if (parent)
		{
			ObjectTransform.Parent = &(parent->ObjectTransform);
			ObjectTransform.Position -= parent->ObjectTransform.GetGlobalPosition();
			ObjectTransform.Rotation -= parent->ObjectTransform.GetGlobalRotation();
		}
		// Process Rotation
		ObjectTransform.Rotation = ProcessAngleVector(ObjectTransform.Rotation);
	}
	void GameObject::AddComponent(ComponentPtr ptr)
	{
		ptr->SetOwner(this);
		m_Components.emplace_back(ptr);
		uint64 componentID = ptr->get_type().get_id();
		ASSERT(m_ComponentMap.find(componentID) == m_ComponentMap.end(), "Trying to add component that exists");
		m_ComponentMap[componentID] = std::move(ptr);
	}

	void GameObject::OnSceneCreatedComponents()
	{
		for (auto& component : m_Components)
		{
			component->OnSceneCreated();
		}
	}
	void GameObject::UpdateComponents()
	{
		for (auto& component : m_Components)
		{
			component->Update();
		}
	}
	void GameObject::PostUpdateComponents()
	{
		for (auto& component : m_Components)
		{
			component->PostUpdate();
		}
	}
	void GameObject::DrawComponents()
	{
		for (auto& component : m_Components)
		{
			component->Draw();
		}
	}
	// Sets to destroy this object at end of frame
	void GameObject::Destroy()
	{
		LOG("Destroying Object. ID: {0}", GetID());
		Objects.DestroyObject(this);
	}
	// Delete all components on destruction
	GameObject::~GameObject()
	{
		for (ComponentPtr comp : m_Components)
		{
			comp->SetAlive(false);
			comp->SetOwner(nullptr);
		}
	}
	// Each object will have a unique identifier based on this static function
	int GameObject::GetNextID()
	{
		static int nextId = 0;
		return nextId++;
	}
}