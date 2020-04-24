#pragma once

namespace Skel
{
	struct PlayerInputState
	{
		bool Forward = false;
		bool Back = false;
		bool Right = false;
		bool Left = false;
		bool Jump = false;
		float Yaw = 0.0f;
	};
}