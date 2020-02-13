#pragma once
#include <EventSystem/Events/ApplicationEvent.h>
#include <EventSystem/Events/MouseEvent.h>
#include <EventSystem/Events/KeyEvent.h>
#include <EventSystem/Subject.h>
#include <Containers/BitArray.h>
#include "Keys.h"

#define Input InputManager::Instance()

namespace Skel
{
	class InputManager : IObserver
	{
	public:
		static InputManager& Instance();
		Subject MouseMoved;
		Subject MouseClicked;
		Subject MouseReleased;
		Subject MouseScrolled;
		Subject KeyPressed;
		Subject KeyReleased;

		void Init();
		void OnEvent(const Subject* subject, class Event& event);
		Vector2 GetMousePosition() const;
		void SetMousePosition(Vector2 mousePos);
		bool IsKeyDown(int keyCode);
		bool IsKeyPressed(int keyCode);
		void ResetKeyPressedStates();
	private:
		static const int MAX_KEYS = 1024;
		InputManager() : m_KeysDown(MAX_KEYS), m_KeysPressed(MAX_KEYS) {};

		
		BitArray m_KeysDown;
		BitArray m_KeysPressed;
		Vector2 m_MousePosition;

		// Callback functions
		bool OnMouseMoved(MouseMovedEvent e);
		bool OnMouseClicked(MouseButtonPressedEvent e);
		bool OnMouseReleased(MouseButtonReleasedEvent e);
		bool OnMouseScrolled(MouseScrolledEvent e);
		bool OnKeyPressed(KeyPressedEvent e);
		bool OnKeyReleased(KeyReleasedEvent e);
	};
}