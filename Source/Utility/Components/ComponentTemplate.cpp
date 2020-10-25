#include "SkelPCH.h"
#include "ComponentTemplate.h"

namespace Skel
{
	ComponentTemplate::ComponentTemplate(rttr::type componentType) : m_Type(componentType)
	{
		// List all types in property type map
		for (auto& property : m_Type.get_properties())
		{
			std::string propertyName = property.get_name().to_string();
			m_PropertyTypeMap[propertyName] = property.get_type().get_name().to_string();
		}
	}

	// Sets a property and then returns itself
	void ComponentTemplate::SetProperty(const std::string& prop, const rttr::variant& var)
	{
		auto propType = m_PropertyTypeMap.find(prop);
		ASSERT(propType != m_PropertyTypeMap.end(), "Property does not exist");
		ASSERT(var.get_type().get_name() == propType->second, "Property type mismatch");
		m_PropertyValueMap[prop] = var;

		return;
	}

	// Creates an instance of a component using data from component template
	// Creates a raw pointer from heap, make sure to delete if calling it outside of object creation code
	ComponentPtr ComponentTemplate::Instatiate()
	{
		rttr::variant newComponentVariant = m_Type.create();

		ComponentPtr newComponent = newComponentVariant.get_value<ComponentPtr>();
		ASSERT(newComponent, "New component failed to initialize");
		for (std::pair<const std::string, rttr::variant>& pair : m_PropertyValueMap)
		{
			// Can I set it from variant? Do I have to convert it? i think so..
			m_Type.set_property_value(pair.first, *newComponent, pair.second);
		}
		return newComponent;
	}
}