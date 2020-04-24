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
		#ifndef SERVER
			ASSERT(m_Model, "Controller does not have valid model");
			m_Model->SetAnimation(name);
			m_CurrentAnimationIndex = m_Model->GetAnimationIndex(name);
		#endif	
		}
		void PlayAnimation(uint8 index) {
		#ifndef SERVER
			ASSERT(m_Model, "Controller does not have valid model");
			m_Model->SetAnimationIndex(index);
		#endif
			m_CurrentAnimationIndex = index;
		}
		uint8 GetCurrentAnimationIndex() const { return m_CurrentAnimationIndex; }
	private:
		ModelAnimationController() : m_Model(nullptr) {}
		ImportedSkinnedModel* m_Model;
		uint8 m_CurrentAnimationIndex = 0;
	};
}