#pragma once
#include <Input/InputState.h>

namespace Skel
{
	struct PlayerInputState : InputState
	{
		bool Forward = false;
		bool Back = false;
	};
}