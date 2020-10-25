#include "SkelPCH.h"
#include <GameObject/GameObject.h>
#include <Resources/ResourceManager.h>
#include "ModelRendererComponent.h"
#include "ModelAnimationComponent.h"

namespace Skel
{
	RTTR_REGISTRATION
	{
		using namespace rttr;
		registration::class_<ModelAnimationComponent>("ModelAnimation")
			.constructor<>(registration::public_access);
	}

	void ModelAnimationComponent::OnCreated()
	{
		// Get skinned model from other component
		ModelRendererComponent& renderer = m_Owner->GetComponent<ModelRendererComponent>();
		ModelPtr model = Resources.GetModel(renderer.GetModelName());
		m_Model = (ImportedSkinnedModel*) &(*model);
		renderer.SetShader("SkinnedModel");
	}
	void ModelAnimationComponent::PlayAnimation(const std::string& name)
	{
#ifndef SERVER
		ASSERT(m_Model, "Controller does not have valid model");
		m_Model->SetAnimation(name);
		m_CurrentAnimationIndex = m_Model->GetAnimationIndex(name);
#endif	
	}
	void ModelAnimationComponent::PlayAnimation(uint8 index) {
#ifndef SERVER
		ASSERT(m_Model, "Controller does not have valid model");
		m_Model->SetAnimationIndex(index);
#endif
		m_CurrentAnimationIndex = index;
	}
}