#pragma once
#include <Components/GameObjectComponent.h>
#include <Graphics/Model/Model.h>
#include <Graphics/OpenGL/Shader.h>


namespace Skel
{

	class ModelRendererComponent : public GameObjectComponent
	{
	public:
		ModelRendererComponent();
		ModelRendererComponent(const std::string& model);

		void SetModel(const std::string& name);
		const std::string& GetModelName() const;

		void SetShader(const std::string& name);
		const std::string& GetShaderName() const;

		virtual void Draw() override;
		
		RTTR_ENABLE(GameObjectComponent)
		RTTR_REGISTRATION_FRIEND
	private:
		std::string m_ModelName;
		std::string m_ShaderName;

		ModelPtr m_Model;
		ShaderPtr m_Shader;
	};
}