#include <skelpch.h>
#include "States/GameStates.h"
#include <GLFW/glfw3.h>
#include <EventSystem/Event.h>
#include <Input/InputManager.h>
#include <EventSystem/Events/KeyEvent.h>
#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#include <chrono>
#include "GameManager.h"

namespace Skel
{
	GameManager& GameManager::Instance()
	{
		static GameManager instance;
		return instance;
	}

	// Called to setup dependencies, namely GLFW
	void GameManager::Init()
	{
		Input.KeyPressed.AddObserver(this);

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(m_Window.m_Window, false);
		ImGui_ImplOpenGL3_Init("#version 410");
	}
	// Called to enter first state
	void GameManager::Start()
	{
	}
	//static WindowProps props;
	GameManager::GameManager() : m_Window(g_WindowProperties),
		m_StateMachine(this)
	{
	}

	GameManager::~GameManager()
	{
	}

	// Main game loop. State Machine contains in-game logic
	void GameManager::Run()
	{
		UpdateDeltaTime();
		#pragma region ImGui Start
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		#pragma endregion

		m_StateMachine.Update();

		#pragma region ImGui End
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)m_Window.GetWidth(), (float)m_Window.GetHeight());
		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		#pragma endregion

		// Poll Window Events
		m_Window.OnUpdate();

		++m_Tick;
	}

	bool GameManager::IsRunning() const
	{
		return m_Running;
	}

	void GameManager::Shutdown()
	{
		LOG("Shutting down game");
		m_Running = false;

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void GameManager::ChangeState(State<GameManager>* state)
	{
		m_StateMachine.ChangeState(state);
	}

	State<GameManager>* GameManager::GetState() const
	{
		return m_StateMachine.CurrentState();
	}

	void GameManager::OnEvent(const Subject * subject, Event & event)
	{
		Evnt::Dispatch<KeyPressedEvent>(event, EVENT_BIND_FN(GameManager, OnKeyPressed));
	}

	// Returns time in seconds
	double GameManager::RunningTime() const
	{
		return glfwGetTime();// double(std::chrono::high_resolution_clock::now().time_since_epoch().count()) / 1000000000.0;
	}

	uint64 GameManager::GetTick() const
	{
		return m_Tick;
	}

	// Returns delta time with scale
	// Will return 0 if paused
	float GameManager::DeltaTime() const
	{
		// Pause / Glitch
		if (m_DeltaTime > 1) {
			return m_TimeScale;
		}
		return m_DeltaTime * m_TimeScale;
	}

	// Returns delta time ignoring time scale, eg. pause
	// Useful for debug methods
	float GameManager::DeltaTimeUnscaled() const
	{
		return m_DeltaTime;
	}

	float GameManager::TimeScale() const
	{
		return m_TimeScale;
	}

	void GameManager::SetTimeScale(float timeScale)
	{
		m_UnpausedTimeScale = timeScale;
		m_TimeScale = timeScale;
	}

	void GameManager::SetTimeScaleFreeze(bool freeze)
	{
		if (!freeze)
			m_TimeScale = m_UnpausedTimeScale;
		else if (m_TimeScale != 0.0f)
		{
			m_UnpausedTimeScale = m_TimeScale;
			m_TimeScale = 0.0f;
		}
	}

	void GameManager::SetPause(bool pause)
	{
		m_Paused = pause;
		SetTimeScaleFreeze(pause);
	}

	bool GameManager::IsPaused() const
	{
		return m_Paused;
	}

	const Window& GameManager::GetWindow() const
	{
		return m_Window;
	}

	Subject& GameManager::GetWindowResizedSubject()
	{
		return m_Window.WindowResized;
	}

	// Delta time allows objects to move despite rendering lag
	void GameManager::UpdateDeltaTime()
	{
		double currentTime = glfwGetTime();
		m_DeltaTime = glm::min(currentTime - m_LastUpdatedTime, 1.0);
		m_LastUpdatedTime = currentTime;
	}

	bool GameManager::OnKeyPressed(KeyPressedEvent& e)
	{
		if (IsPaused())
			return false;

		switch (e.KeyCode)
		{
		case KEY_X:
			SetTimeScaleFreeze(m_TimeScale != 0.0f);
			Game.GetWindow().SetCursorEnabled(m_TimeScale == 0.0f);
			break;
		}
		return false;
	}

}