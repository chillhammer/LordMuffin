#include <skelpch.h>
#include "InputManager.h"

namespace Skel
{
	InputManager& InputManager::Instance()
	{
		static InputManager input;
		return input;
	}

	// Allows to force Singleton lazy instantiation 
	void InputManager::Init()
	{
		MouseMoved.AddObserver(this);
		MouseClicked.AddObserver(this);
		MouseReleased.AddObserver(this);
		MouseScrolled.AddObserver(this);
		KeyPressed.AddObserver(this);
		KeyReleased.AddObserver(this);
		m_MousePosition = Vector2(0, 0);
	}

	// Handles Input Changes. Dispatch events to appropriate callback
	void InputManager::OnEvent(const Subject * subject, Event & event)
	{
		Evnt::Dispatch<MouseMovedEvent>(event,			EVENT_BIND_FN(InputManager, OnMouseMoved));
		Evnt::Dispatch<MouseButtonPressedEvent>(event,	EVENT_BIND_FN(InputManager, OnMouseClicked));
		Evnt::Dispatch<MouseButtonReleasedEvent>(event, EVENT_BIND_FN(InputManager, OnMouseReleased));
		Evnt::Dispatch<MouseScrolledEvent>(event,		EVENT_BIND_FN(InputManager, OnMouseScrolled));
		Evnt::Dispatch<KeyPressedEvent>(event,			EVENT_BIND_FN(InputManager, OnKeyPressed));
		Evnt::Dispatch<KeyReleasedEvent>(event,			EVENT_BIND_FN(InputManager, OnKeyReleased));
	}

	// Getter for Mouse Position
	Vector2 InputManager::GetMousePosition() const
	{
		return m_MousePosition;
	}

	// Only changes local variable, not actual cursor position
	void InputManager::SetMousePosition(Vector2 mousePos)
	{
		m_MousePosition = mousePos;
	}

	bool InputManager::IsKeyDown(int keyCode)
	{
		return m_KeysDown.GetBit(keyCode);
	}

	bool InputManager::IsKeyPressed(int keyCode)
	{
		return m_KeysPressed.GetBit(keyCode);
	}

	// Should be done once per frame before reading input
	void InputManager::ResetKeyPressedStates()
	{
		m_KeysPressed.Clear();
	}

	#pragma region Callbacks
	bool InputManager::OnMouseMoved(MouseMovedEvent e)
	{
		Vector2 newMousePos(e.MouseX, e.MouseY);
		m_MousePosition = std::move(newMousePos);
		return false;
	}

	bool InputManager::OnMouseClicked(MouseButtonPressedEvent e)
	{
		return false;
	}

	bool InputManager::OnMouseReleased(MouseButtonReleasedEvent e)
	{
		return false;
	}

	bool InputManager::OnMouseScrolled(MouseScrolledEvent e)
	{
		return false;
	}

	bool InputManager::OnKeyPressed(KeyPressedEvent e)
	{
		int key = e.KeyCode;
		if (key == -1) return false;
		m_KeysDown.SetBit(key);
		m_KeysPressed.SetBit(key);
		ASSERT(m_KeysPressed.GetBit(key) > 0, "Must be true since you just set it");
		return false;
	}

	bool InputManager::OnKeyReleased(KeyReleasedEvent e)
	{
		int key = e.KeyCode;
		m_KeysDown.ClearBit(key);
		return false;
	}
	#pragma endregion

}