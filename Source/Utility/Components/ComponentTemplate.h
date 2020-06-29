#pragma once
#include <Macros.h>
#include <map>
#include <string>
#include <rttr/variant.h>
#include <rttr/type.h>
#include <Components/GameObjectComponent.h>

namespace Skel
{
	class ComponentTemplate;
	typedef std::shared_ptr<ComponentTemplate> ComponentTemplatePtr;

	// Data only for a component
	// Allows components to be stored in a scene to be loaded in the future
	class ComponentTemplate
	{
	public:
		ComponentTemplate() : m_Type(rttr::type::get<GameObjectComponent>()) {};
		ComponentTemplate(rttr::type);
		void SetProperty(const std::string& prop, const rttr::variant& var);

		ComponentPtr Instatiate();

		rttr::type m_Type;
	private:
		
		std::map<std::string, std::string> m_PropertyTypeMap;
		std::map<std::string, rttr::variant> m_PropertyValueMap;
	};
}