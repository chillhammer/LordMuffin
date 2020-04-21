#pragma once
#include <Macros.h>
#include "ImportedSkinnedModel.h"

namespace Skel
{
	// Wrapper class for skinned models used by GameObjects to control animations
	class ModelAnimationController
	{
	public:
		ModelAnimationController(ModelPtr m) : m_Model((ImportedSkinnedModel*) &(*m)) {}

		void PlayAnimation(const std::string& name) {
			ASSERT(m_Model, "Controller does not have valid model");
			m_Model->SetAnimation(name);
			m_CurrentAnimationIndex = m_Model->GetAnimationIndex(name);
		}
		void PlayAnimation(uint8 index) {
			ASSERT(m_Model, "Controller does not have valid model");
			m_Model->SetAnimationIndex(index);
			m_CurrentAnimationIndex = index;
		}
	private:
		ModelAnimationController() : m_Model(nullptr) {}
		ImportedSkinnedModel* m_Model;
		uint8 m_CurrentAnimationIndex;
	};
}