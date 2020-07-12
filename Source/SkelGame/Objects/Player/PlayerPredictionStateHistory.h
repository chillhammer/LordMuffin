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
	// This allows for client side prediction and will correct the player if needed
	class PlayerPredictionStateHistory
	{
	public:
		void RecordState(PlayerInputState input, PlayerSnapshotState state);
		void RemoveOldest();

		bool CorrectState(const PlayerSnapshotState& corrected, GameObject* obj, double time, uint64 clientTick);
		std::vector<PlayerInputState> RecentInputs(uint8 num) const;
	private:

		PredictedMove m_PredictedMoves[Net::PREDICTED_STATES];
		PredictedMoveResult m_PredictedMoveResults[Net::PREDICTED_STATES];
		double m_LastValidatedTime = 0;

		uint32 m_Start;
		uint32 m_End;
		uint32 m_Count;
	};
}