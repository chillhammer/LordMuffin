#pragma once
#include <Components/GameObjectComponent.h>
#include <Graphics/Model/ImportedSkinnedModel.h>


namespace Skel
{

	class ModelAnimationComponent : public GameObjectComponent
	{
	public:
		ModelAnimationComponent() : m_Model(nullptr) {}

		virtual void OnCreated() override;

		void PlayAnimation(const std::string& name);
		void PlayAnimation(uint8 index);

	private:
		ImportedSkinnedModel* m_Model;
		uint8 m_CurrentAnimationIndex = 0;

		RTTR_ENABLE(GameObjectComponent)
		RTTR_REGISTRATION_FRIEND
	};
}