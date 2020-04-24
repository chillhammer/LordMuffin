#pragma once
#include <Macros.h>
#include "PlayerObject.h"

namespace Skel
{
	struct PlayerSnapshotState
	{
		PlayerSnapshotState() {};
		PlayerSnapshotState(const PlayerObject& obj) : Position(obj.ObjectTransform.Position), 
			Yaw(obj.ObjectTransform.GetYaw()), Pitch(obj.ObjectTransform.GetPitch()),
			AnimationIndex(obj.GetAnimation()) {};
		Vector3 Position;
		float Pitch = 0;
		float Yaw = 0;
		uint8 AnimationIndex = 0;
	};
}