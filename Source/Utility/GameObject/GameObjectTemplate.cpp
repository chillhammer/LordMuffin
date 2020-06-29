#include "SkelPCH.h"
#include "GameObjectTemplate.h"

namespace Skel
{
	GameObjectTemplate::GameObjectTemplate() : Scale(1.0f)
	{
	}
	ComponentTemplatePtr GameObjectTemplate::AddComponentTemplate(const std::string& component)
	{
		rttr::type componentType = rttr::type::get_by_name(component);
		ComponentTemplate* compTemp = new ComponentTemplate(componentType);
		ComponentTemplatePtr newComponentPtr(compTemp);
		if (newComponentPtr)
		{
			m_ComponentTemplates.emplace_back(newComponentPtr);
		}
		return newComponentPtr;
	}
	GameObjectPtr GameObjectTemplate::Instantiate()
	{
		GameObjectPtr obj(new GameObject());
		obj->ObjectTransform.Position = Position;
		obj->ObjectTransform.Rotation = Rotation;
		obj->ObjectTransform.Scale = Scale;

		for (auto& compTemp : m_ComponentTemplates)
		{
			ComponentPtr comp = compTemp->Instatiate();
			obj->AddComponent(comp);
		}

		for (ComponentPtr& comp : obj->m_Components)
		{
			comp->OnCreated();
		}
		
		return obj;
	}
}
