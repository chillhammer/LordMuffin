#pragma once
#include <EventSystem/Event.h>
#include <EventSystem/Subject.h>
#include <EventSystem/Events/ApplicationEvent.h>

struct GLFWwindow;
namespace Skel
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Game Title Here",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	/**
		Window class is a wrapper for GLFW's window, using m_Data to transfer
		data between callback function and main class properties
	**/
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window(const WindowProps& props);
		~Window();

		void OnUpdate();

		unsigned int GetWidth() const { return m_Data.Width; };
		unsigned int GetHeight() const { return m_Data.Height; }

		// Attributes
		// Must set Event Callback or will crash the game
		virtual void SetEventCallback(const EventCallbackFn& callback) {
			m_Data.EventCallback = callback;
		};
		void SetVSync(bool enabled) { m_Data.VSync = enabled; };
		bool IsVSync() const { return m_Data.VSync; }

		void SetCursorEnabled(bool enabled);
		bool IsCursorEnabled() const
		{
			return m_Data.CursorEnabled;
		}
		void SetCursorPosition(float x, float y) const;

		bool OnWindowResized(WindowResizeEvent& e);
		GLFWwindow* m_Window;
		Subject WindowResized;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			bool CursorEnabled;
			EventCallbackFn EventCallback;
		};
		WindowData m_Data;
	private:
		void OnEvent(Event& e);
	};
}