#pragma once
#include <Macros.h>
#include "PlayerComponent.h"

namespace Skel
{
	struct PlayerSnapshotState
	{
		PlayerSnapshotState() : Position(0.0f) {};
		PlayerSnapshotState(const PlayerComponent& obj, bool movedByServer) : Position(obj.GetOwner()->ObjectTransform.Position), 
			Velocity(obj.GetVelocity()),
			Yaw(obj.GetOwner()->ObjectTransform.GetYaw()), Pitch(obj.GetOwner()->ObjectTransform.GetPitch()),
			AnimationIndex(obj.GetCurrentAnimationIndex()),
			OverlayAnimationIndex(obj.GetOverlayAnimationIndex()),
			MovedByServer(movedByServer) {};
		Vector3 Position;
		Vector3 Velocity;
		float Pitch = 0;
		float Yaw = 0;
		uint8 AnimationIndex = 0;
		uint8 OverlayAnimationIndex = 0;
		bool MovedByServer = false;
	};
}