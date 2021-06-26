#pragma once
#include <Macros.h>
#include "PlayerComponent.h"

namespace Skel
{
	struct PlayerSnapshotState
	{
		PlayerSnapshotState() : Position(0.0f) {};
		PlayerSnapshotState(const PlayerComponent& obj) : Position(obj.GetOwner()->ObjectTransform.Position), 
			Yaw(obj.GetOwner()->ObjectTransform.GetYaw()), Pitch(obj.GetOwner()->ObjectTransform.GetPitch()),
			AnimationIndex(obj.GetCurrentAnimationIndex()),
			OverlayAnimationIndex(obj.GetOverlayAnimationIndex()) {};
		Vector3 Position;
		float Pitch = 0;
		float Yaw = 0;
		uint8 AnimationIndex = 0;
		uint8 OverlayAnimationIndex = 0;
	};
}