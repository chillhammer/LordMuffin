#pragma once
#include "PlayerObject.h"
#include "PlayerSnapshotState.h"
#include "Net/Net.h"

namespace Skel {

	struct PredictedMove {
		PlayerInputState Input;
		double DeltaTime;
		PredictedMove() : Input(), DeltaTime(0) {};
		PredictedMove(PlayerInputState input, double dt) : Input(input), DeltaTime(dt) {};
	};

	struct PredictedMoveResult {
		PlayerSnapshotState State;
		double Time;
		PredictedMoveResult() : State(), Time(0) {};
		PredictedMoveResult(PlayerSnapshotState state, double time) : State(state), Time(time) {};
	};

	// Stores the input and state info of the player 
	class PlayerPredictionStateHistory
	{
	public:


	private:
		PredictedMove m_PredictedMoves[Net::PREDICTED_STATES];
		PredictedMoveResult m_PredictedMoveResults[Net::PREDICTED_STATES];
	};
}