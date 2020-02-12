#pragma once
#include <Macros.h>
#include "PlayerObject.h"

namespace Skel
{
	struct PlayerSnapshotState
	{
		PlayerSnapshotState() {};
		PlayerSnapshotState(const PlayerObject& obj) : Position(obj.ObjectTransform.Position) {};
		Vector3 Position;
		float Pitch = 0;
		float Yaw = 0;
	};
}