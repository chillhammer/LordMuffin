#include "SkelPCH.h"
#include <Resources/ResourceManager.h>
#include <Input/InputManager.h>
#include <GameObject/GameObject.h>
#include <Game/States/GameStates.h>
#include "CameraComponent.h"

namespace Skel
{
	RTTR_REGISTRATION
	{
		using namespace rttr;
		registration::class_<CameraComponent>("Camera")
					 .constructor<>(registration::public_access)
					 .property("Shaders", &CameraComponent::GetShaderList, &CameraComponent::SetShaderList);
	}
	CameraComponent::CameraComponent()
			:	m_Speed(7.8f), m_Sensitivity(12.f), m_Yaw(180.0f), m_TargetYaw(180),
				Mode(CameraMode::FirstPerson), m_PivotLength(4.9f), m_PivotOffset(0, 2.8f, 0.4f),
				m_PivotPosition(0, 0, 0), m_DeltaMousePosition(0.0f, 0.0f), m_Pitch(0.0f), m_PrevMousePosition(0.0f, 0.0f),
				m_TargetLocation(0.0f, 0.0f, 0.0f)
	{
	}
	void Skel::CameraComponent::SetShaderList(const std::vector<std::string>& list)
	{
		m_ShaderNames = list;
		m_Shaders.reserve(list.size());
		for (const std::string& name : list)
		{
			ShaderPtr shader = Resources.GetShader(name);
			m_Shaders.emplace_back(std::move(shader));
		}
	}
	const std::vector<std::string>& Skel::CameraComponent::GetShaderList() const
	{
		return m_ShaderNames;
	}
	Matrix4x4 Skel::CameraComponent::GetViewMatrix() const
	{
		return glm::lookAt(m_Owner->ObjectTransform.Position, m_Owner->ObjectTransform.Position + m_Owner->ObjectTransform.GetHeading(), m_Owner->ObjectTransform.GetUp());;
	}
	void Skel::CameraComponent::SetPivotPosition(Vector3 newPosition)
	{
		m_PivotPosition = newPosition;
		UpdatePivotPosition();
	}
	void Skel::CameraComponent::LookInDirection(Vector3 direction)
	{
		direction = glm::normalize(direction);
		float newYaw = glm::degrees(std::atan2(direction.z, direction.x)) + 90;
		float newPitch = glm::degrees(std::atan2(direction.y, direction.x));
		m_Yaw = newYaw;
		m_Pitch = newPitch;
	}
	void Skel::CameraComponent::OnCreated()
	{
		LOG("Initialized Camera");
		Input.MouseMoved.AddObserver(this);
		Input.KeyPressed.AddObserver(this);
		m_PrevMousePosition = Input.GetMousePosition();

		// Invisible Cursor
		Game.GetWindow().SetCursorEnabled(false);
	}
	void Skel::CameraComponent::PostUpdate()
	{
		// First Person Movement
		if (Mode == CameraMode::NoClip)
		{
			if (Input.IsKeyDown(KEY_W))
			{
				m_Owner->ObjectTransform.Position += m_Owner->ObjectTransform.GetHeading() * m_Speed * float(Game.DeltaTimeUnscaled());
			}
			if (Input.IsKeyDown(KEY_S))
			{
				m_Owner->ObjectTransform.Position -= m_Owner->ObjectTransform.GetHeading() * m_Speed * float(Game.DeltaTimeUnscaled());
			}
			if (Input.IsKeyDown(KEY_D))
			{
				m_Owner->ObjectTransform.Position += m_Owner->ObjectTransform.GetSide() * m_Speed * float(Game.DeltaTimeUnscaled());
			}
			if (Input.IsKeyDown(KEY_A))
			{
				m_Owner->ObjectTransform.Position -= m_Owner->ObjectTransform.GetSide() * m_Speed * float(Game.DeltaTimeUnscaled());
			}
			if (Input.IsKeyDown(KEY_LEFT_SHIFT))
			{
				m_Owner->ObjectTransform.Position -= Vector3(0.f, 2.f, 0.f) * m_Speed * float(Game.DeltaTimeUnscaled());
			}
			if (Input.IsKeyDown(KEY_SPACE))
			{
				m_Owner->ObjectTransform.Position += Vector3(0.f, 2.f, 0.f) * m_Speed * float(Game.DeltaTimeUnscaled());
			}

			if (m_DeltaMousePosition.x > 1.0f || m_DeltaMousePosition.x < -1.0f)
			{
				int a = 1;
			}

			// Look Around
			m_Pitch -= m_DeltaMousePosition.y * m_Sensitivity * static_cast<float>(Game.DeltaTimeUnscaled());
			m_Yaw += m_DeltaMousePosition.x * m_Sensitivity * static_cast<float>(Game.DeltaTimeUnscaled());
			
			m_Pitch = glm::clamp<float>(m_Pitch, -89, 89);

			if (m_DeltaMousePosition.x > 1.0f || m_DeltaMousePosition.x < -1.0f)
			{
				//LOG("Delta.X: {0}", m_DeltaMousePosition.x * m_Sensitivity * static_cast<float>(Game.DeltaTimeUnscaled()));
				if (m_DeltaMousePosition.x * m_Sensitivity * static_cast<float>(Game.DeltaTimeUnscaled()) > 10.0f)
				{
					int a = 1;
				}
			}
			else
			{
				// LOG("Delta.X: {0}", m_DeltaMousePosition.x);
			}

			m_Owner->ObjectTransform.SetPitch(m_Pitch);
			m_Owner->ObjectTransform.SetYaw(m_Yaw);
		}
		// Third-Person Pivoting or First Person
		if (Mode == CameraMode::Pivot || Mode == CameraMode::FirstPerson)
		{
			// Look Around
			m_Pitch -= m_DeltaMousePosition.y * m_Sensitivity * static_cast<float>(Game.DeltaTimeUnscaled());
			m_Yaw += m_DeltaMousePosition.x * m_Sensitivity * static_cast<float>(Game.DeltaTimeUnscaled());
			m_Pitch = glm::clamp<float>(m_Pitch, -10, -10);

			m_Owner->ObjectTransform.SetPitch(m_Pitch);
			m_Owner->ObjectTransform.SetYaw(m_Yaw);

			UpdatePivotPosition();
		}
		// Reset Delta Movement
		m_DeltaMousePosition = Vector2(0, 0);
	}
	// Updating shaders with camera information
	void Skel::CameraComponent::Draw()
	{
		for (ShaderPtr& shader : m_Shaders)
		{
			shader->Bind();
			shader->SetUniformMat4f("u_ViewProjection", Camera::GetProjectionMatrix() * GetViewMatrix());
		}
	}
	void Skel::CameraComponent::OnEvent(const Subject* subject, Event& e)
	{
		Evnt::Dispatch<MouseMovedEvent>(e, EVENT_BIND_FN(CameraComponent, OnMouseMoved));
		Evnt::Dispatch<KeyPressedEvent>(e, EVENT_BIND_FN(CameraComponent, OnKeyPressed));
	}
	// Moving camera with mouse
	static int DELTA_CAP = 300;
	bool Skel::CameraComponent::OnMouseMoved(MouseMovedEvent& e)
	{
		if (Game.GetState() == GameStates::MainMenu::Instance()) return false;
		//LOG("Camera: Mouse Moved: {0}, {1}", e.MouseX, e.MouseY);
		float midWindowX = Game.GetWindow().GetWidth() * 0.5f;
		float midWindowY = Game.GetWindow().GetHeight() * 0.5f;
		if (Input.GetMousePosition() != Vector2(0, 0) && !Game.IsPaused()) // Initial Mouse Delta Jump Ignored
		{
			m_DeltaMousePosition = Vector2(e.MouseX - midWindowX, e.MouseY - midWindowY);
			if (glm::length2(m_DeltaMousePosition) > DELTA_CAP * DELTA_CAP || Game.TimeScale() == 0.0f)
				m_DeltaMousePosition = Vector2(0.0f, 0.0f);
		}
		if (m_DeltaMousePosition.x > 1.0f || m_DeltaMousePosition.x < -1.0f)
		{
			//LOG("Delta.X: {0}", m_DeltaMousePosition.x);
			if (m_DeltaMousePosition.x > 30.0f || m_DeltaMousePosition.x < -30.0f)
			{
			//	int a = 1;
			}
		}
		if (!Game.IsPaused() && Game.TimeScale() != 0.0f)
			Game.GetWindow().SetCursorPosition(midWindowX, midWindowY);
		return false;
	}
	// Camera mode switching
	bool Skel::CameraComponent::OnKeyPressed(KeyPressedEvent& e)
	{
		if (Game.IsPaused()) return false;
		switch (e.KeyCode)
		{
		case KEY_C:
			Mode = (Mode == CameraMode::NoClip ? CameraMode::FirstPerson : CameraMode::NoClip);
			break;
		case KEY_B:
			Mode = (Mode == CameraMode::Pivot ? CameraMode::FirstPerson : CameraMode::Pivot);
			break;
		case KEY_V:
			if (Mode == CameraMode::Frozen)
				Mode = CameraMode::NoClip;
			else
				Mode = CameraMode::Frozen;
			break;

		case KEY_ESCAPE:
			if (Mode == CameraMode::NoClip && Game.IsPaused())
				Mode = CameraMode::Frozen;
			else if (Mode == CameraMode::Frozen && !Game.IsPaused())
				Mode = CameraMode::NoClip;
			break;
		}
		return false;
	}
	// Helper function to stick to pivot pos
	void Skel::CameraComponent::UpdatePivotPosition()
	{
		if (Mode != CameraMode::Pivot && Mode != CameraMode::FirstPerson)
			return;
		Vector3 pivotStick = m_PivotLength * -1 * m_Owner->ObjectTransform.GetHeading() * (Mode == CameraMode::Pivot ? 1.0f : 0.0f);
		Matrix4x4 rotMat = glm::eulerAngleXYZ(0.f, glm::radians(-m_Owner->ObjectTransform.GetYaw()), 0.f);
		Vector3 rotatedPivotOffset = rotMat * Vector4(m_PivotOffset, 1);
		m_Owner->ObjectTransform.Position = m_PivotPosition + rotatedPivotOffset + pivotStick;
	}

	namespace Camera
	{
		Matrix4x4 GetProjectionMatrix()
		{
			float fov = 45;
			float aspect = 0.1f;
			if (Game.GetWindow().GetHeight() != 0 && Game.GetWindow().GetHeight() != 0)
				aspect = (float)Game.GetWindow().GetWidth() / (float)Game.GetWindow().GetHeight();

			return glm::perspective(glm::radians(fov), aspect, 0.1f, 100.f);
		}
		Matrix4x4 GetOrthographicMatrix()
		{
			return glm::ortho(0.0f, (float)Game.GetWindow().GetWidth(), (float)Game.GetWindow().GetHeight(), 0.0f);
		}
	}
}
