#include <skelpch.h>
#include <Resources/ResourceManager.h>
#include <EventSystem/Events/KeyEvent.h>
#include <EventSystem/Event.h>
#include <Game/States/GameStates.h>
#include <Audio/AudioEngine.h>
#include <Input/Keys.h>
#include "PauseScreen.h"

namespace Skel
{
	PauseScreen::PauseScreen() :
		m_Font(Resources.GetFont("InGame")),
		m_Backdrop(Vector4(0.0f, 0.0f, 0.0f, 0.5f)), m_Resume(), m_Takeover(), m_Exit(),
		m_Enabled(false)
	{
		float ww = Game.GetWindow().GetWidth();
		float wh = Game.GetWindow().GetHeight();
		m_Backdrop.UITransform.Scale = Vector3(ww, wh, 0.1f);
		m_Backdrop.UITransform.Position.z = 0.01f;
		Game.GetWindowResizedSubject().AddObserver(this);

		float bWidth = 400.0f;
		float bHeight = 100.0f;
		float bPad = 20.0f;
		float totalHeight = (bHeight + bPad) * 3.0f;
		float bx = ww * 0.5f - bWidth * 0.5f;
		float by = wh * 0.5f - totalHeight * 0.5f;
		
		Vector3 buttonScale = Vector3(bWidth, bHeight, 1.0f);
		float byy = by;
		// Resume
		m_Resume.SetProperties(bx, byy, bWidth, bHeight, "Resume Game", [&] {
			ToggleScreen();
		});
		byy += bHeight + bPad;

		// Music
		m_Music.SetProperties(bx, byy, bWidth, bHeight, "Sound: On", [&] {
			Audio.SetAudioEnabled(!Audio.GetAudioEnabled());
			// Change label
			std::string label = (Audio.GetAudioEnabled() ? "Sound: On" : "Sound: Off");
			m_Music.SetLabel(label);
		});

		byy += bHeight + bPad;
		// Exit
		m_Exit.SetProperties(bx, byy, bWidth, bHeight, "Exit to Main Menu", [&] {
			ToggleScreen();
			Game.ChangeState(GameStates::MainMenu::Instance());
		});
		// Adding Event Listener
		Input.KeyPressed.AddObserver(this);
	}
	void PauseScreen::ToggleScreen()
	{
		m_Enabled = !m_Enabled;
		Game.SetPause(m_Enabled);
		Game.GetWindow().SetCursorEnabled(m_Enabled);
	}
	bool PauseScreen::IsEnabled()
	{
		return m_Enabled;
	}
	void PauseScreen::UpdateAndDraw()
	{
		if (!m_Enabled) return;

		m_Backdrop.Draw();
		m_Resume.UpdateAndDraw();
		m_Takeover.UpdateAndDraw();
		m_Music.UpdateAndDraw();
		m_Exit.UpdateAndDraw();

		// Make Sure Game is Paused
		if (!Game.IsPaused())
			m_Enabled = false;
	}
	void PauseScreen::OnEvent(const Subject* subject, Event& event)
	{
		Evnt::Dispatch<KeyPressedEvent>(event, EVENT_BIND_FN(PauseScreen, OnKeyPressed));
		Evnt::Dispatch<WindowResizeEvent>(event, EVENT_BIND_FN(PauseScreen, OnWindowResized));
	}
	bool PauseScreen::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.KeyCode == KEY_ESCAPE)
		{
			ToggleScreen();
		}
		return false;
	}
	// Makes the screen scale. Note: currently duplicating hard-coded spacing values
	bool PauseScreen::OnWindowResized(WindowResizeEvent& e)
	{
		float ww = Game.GetWindow().GetWidth();
		float wh = Game.GetWindow().GetHeight();

		// Update Positions
		m_Backdrop.UITransform.Scale = Vector3(ww, wh, 0.1f);

		float bWidth = 400.0f;
		float bHeight = 100.0f;
		float bPad = 20.0f;
		float totalHeight = (bHeight + bPad) * 4.0f;
		float bx = ww * 0.5f - bWidth * 0.5f;
		float by = wh * 0.5f - totalHeight * 0.5f;

		Vector3 buttonScale = Vector3(bWidth, bHeight, 1.0f);
		float byy = by;
		// Resume
		m_Resume.UITransform.Position = Vector3(bx, byy, 0.1f);
		byy += bHeight + bPad;
		// Takeover
		m_Takeover.UITransform.Position = Vector3(bx, byy, 0.1f);
		byy += bHeight + bPad;

		// Music
		m_Music.UITransform.Position = Vector3(bx, byy, 0.1f);

		byy += bHeight + bPad;
		// Exit
		m_Exit.UITransform.Position = Vector3(bx, byy, 0.1f);

		return false;
	}
}