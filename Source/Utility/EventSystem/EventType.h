#pragma once
#include <skelpch.h>
#include <Events/GameEventTypes.h>
#include <Macros.h>

// Centralized approach to keeping track of all events
// Considered using strings over enums, but ultimately decided this game
//	was small enough.
enum class EventType
{
	None = 0,
	WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
	AppTick, AppUpdate, AppRender,
	KeyPressed, KeyReleased, KeyTyped,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,

	ClientDisconnect,ClientConnect,

	CUSTOM_EVENTS
};