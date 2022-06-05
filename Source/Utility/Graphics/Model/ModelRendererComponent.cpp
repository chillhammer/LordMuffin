#include "SkelPCH.h"
#include <Resources/ResourceManager.h>
#include <GameObject/GameObject.h>
#include <rttr/registration>
#include "ModelRendererComponent.h"

namespace Skel
{
	RTTR_REGISTRATION
	{
		using namespace rttr;
		registration::class_<ModelRendererComponent>("ModelRenderer")
					 .constructor<>(registration::public_access)
					 .property("Model", &ModelRendererComponent::GetModelName, &ModelRendererComponent::SetModel)
					 .property("Shader", &ModelRendererComponent::GetShaderName, &ModelRendererComponent::SetShader);
	}

	Skel::ModelRendererComponent::ModelRendererComponent() : m_Shader(Resources.GetShader("Model"))
	{
	}

	Skel::ModelRendererComponent::ModelRendererComponent(const std::string& model) : m_Model(Resources.GetModel(model)),
		m_ModelName(model), m_Shader(Resources.GetShader("Model"))
	{
	}

	void Skel::ModelRendererComponent::SetModel(const std::string& name)
	{
		m_ModelName = name;
		m_Model = Resources.GetModel(name);
	}
	const std::string& Skel::ModelRendererComponent::GetModelName() const
	{
		return m_ModelName;
	}
	void Skel::ModelRendererComponent::SetShader(const std::string& name)
	{
		m_ShaderName = name;
		m_Shader = Resources.GetShader(name);
	}
	const std::string& Skel::ModelRendererComponent::GetShaderName() const
	{
		return m_ShaderName;
	}
	void ModelRendererComponent::Draw()
	{
		GameObject* owner = GetOwner();
		ASSERT(owner, "Must have component owner");
		ASSERT(m_Model, "Must have valid model pointer");
		m_Model->Draw(m_Shader, owner->ObjectTransform.GetMatrix());
	}
}
