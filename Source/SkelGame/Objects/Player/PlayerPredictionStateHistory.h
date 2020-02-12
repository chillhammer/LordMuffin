#pragma once
#include "PlayerObject.h"
#include "PlayerSnapshotState.h"
#include "Net/Net.h"

namespace Skel {

	struct PredictedMove {
		PlayerInputState InputState;
		double DeltaTime;
		PredictedMove() : InputState(), DeltaTime(0) {};
		PredictedMove(PlayerInputState input, double dt) : InputState(input), DeltaTime(dt) {};
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
		void RecordState(PlayerInputState input, PlayerSnapshotState state);
		void RemoveOldest();

		bool CorrectState(const PlayerSnapshotState& corrected, PlayerObject& obj, double time);

	private:
		PredictedMove m_PredictedMoves[Net::PREDICTED_STATES];
		PredictedMoveResult m_PredictedMoveResults[Net::PREDICTED_STATES];
		double m_LastValidatedTime = 0;

		uint32 m_Start;
		uint32 m_End;
		uint32 m_Count;
	};
}