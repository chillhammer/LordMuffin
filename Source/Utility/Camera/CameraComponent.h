#pragma once
#include <EventSystem/IObserver.h>
#include <Components/GameObjectComponent.h>

namespace Skel
{
	class CameraComponent : public GameObjectComponent, IObserver
	{
	public:
		CameraComponent();


		void SetShaderList(const std::vector<std::string>& list);
		const std::vector<std::string>& GetShaderList() const;

		Matrix4x4 GetViewMatrix() const;
		void SetPivotPosition(Vector3 newPosition);
		void LookInDirection(Vector3 direction);

		virtual void OnCreated() override;
		virtual void PostUpdate() override;
		virtual void Draw() override;

		void OnEvent(const Subject* subject, Event& e);
	public:
		CameraMode Mode;
	private:
		float m_Speed;
		float m_Sensitivity;
		Vector2 m_PrevMousePosition;
		Vector2 m_DeltaMousePosition;
		float m_Pitch;
		float m_Yaw;
		float m_TargetYaw;
		Vector3 m_PivotPosition;
		Vector3 m_PivotOffset;
		float m_PivotLength;

		Vector3 m_TargetLocation;
	private:
		bool OnMouseMoved(class MouseMovedEvent& e);
		bool OnKeyPressed(class KeyPressedEvent& e);
		void UpdatePivotPosition();


		RTTR_ENABLE(GameObjectComponent)
		RTTR_REGISTRATION_FRIEND
	private:
		std::vector<ShaderPtr> m_Shaders;
		std::vector<std::string> m_ShaderNames;
	};
}