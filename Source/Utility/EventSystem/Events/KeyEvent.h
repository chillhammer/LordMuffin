#pragma once

#include <EventSystem/Event.h>


namespace Skel
{
	/**
		A Key Event is related to keyboard input.
		All of these events must have a keycode.
	**/
	EVENT_CLASS_SIMPLE(KeyPressed,
		KeyPressedEvent(ARGS(int key, int repeat)) : ARGS(KeyCode(key), RepeatCount(repeat)) {};
		int KeyCode;
		int RepeatCount;
	);

	EVENT_CLASS_SIMPLE(KeyReleased,
		KeyReleasedEvent(int key) : KeyCode(key) {};
		int KeyCode;
	);

	EVENT_CLASS_SIMPLE(KeyTyped,
		KeyTypedEvent(ARGS(int key, int repeat)) : ARGS(KeyCode(key), RepeatCount(repeat)) {};
		int KeyCode;
		int RepeatCount;
	);
}