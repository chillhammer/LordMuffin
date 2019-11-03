#pragma once
#include <Game/GameManager.h>
#include <Input/InputManager.h>
#include <UI/Button.h>
#include <Graphics/Text/Font.h>

namespace Skel
{
	class PauseScreen : public IObserver
	{
	public:
		PauseScreen();

		void ToggleScreen();
		bool IsEnabled();
		void UpdateAndDraw();

		virtual void OnEvent(const Subject* subject, class Event& event) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		FontPtr m_Font;
		UIElement m_Backdrop;
		Button m_Resume;
		Button m_Takeover;
		Button m_Music;
		Button m_Exit;
		bool m_Enabled;
	};
}