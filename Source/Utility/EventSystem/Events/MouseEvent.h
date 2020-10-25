#pragma once

#include <EventSystem/Event.h>


namespace Skel
{
	EVENT_CLASS_SIMPLE(MouseMoved,
		MouseMovedEvent(ARGS(double mx, double my)) : ARGS(MouseX(mx), MouseY(my)) {};
		double MouseX;
		double MouseY;
	);

	EVENT_CLASS_SIMPLE(MouseScrolled,
		MouseScrolledEvent(ARGS(float mx, float my)) : ARGS(MouseYOffset(mx), MouseXOffset(my)) {};
		float MouseYOffset;
		float MouseXOffset;
	);

	EVENT_CLASS_SIMPLE(MouseButtonPressed,
		MouseButtonPressedEvent(int b) : Button(b) {};
		int Button;
	);

	EVENT_CLASS_SIMPLE(MouseButtonReleased,
		MouseButtonReleasedEvent(int b) : Button(b) {};
		int Button;
	);

}