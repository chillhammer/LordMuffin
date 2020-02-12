#include "SkelPCH.h"
#include <Game/GameManager.h>
#include "PlayerPredictionStateHistory.h"
#include <Profiler/ProfileBlock.h>

namespace Skel {
	void PlayerPredictionStateHistory::RecordState(PlayerInputState input, PlayerSnapshotState state)
	{
		// If full, delete oldest
		if (m_Count >= Net::PREDICTED_STATES)
			RemoveOldest();

		PredictedMove& move = m_PredictedMoves[m_End];
		PredictedMoveResult& moveResult = m_PredictedMoveResults[m_End];

		move.DeltaTime = Game.DeltaTime();

		move.InputState = input;

		moveResult.Time = Game.RunningTime();
		moveResult.State = state;

		m_Count++;
		m_End = (m_End + 1) % Net::PREDICTED_STATES;
		ASSERT(m_Count <= Net::PREDICTED_STATES, "Count must be within bounds");
	}
	void PlayerPredictionStateHistory::RemoveOldest()
	{
		ASSERT(m_Count > 0, "Cannot remove from empty history");
		m_Start = (m_Start + 1) % Net::PREDICTED_STATES;

		m_Count--;
	}
	// Will see if it needs to correct state, then returns new state with input applied
	// Time parameter is time to look for
	bool PlayerPredictionStateHistory::CorrectState(const PlayerSnapshotState& corrected, PlayerObject& player, double time)
	{
		// If nothing to compare to, snap to 'corrected' state 
		if (m_Count == 0 || (m_PredictedMoveResults[m_Start].Time > time && m_LastValidatedTime < time)) {
			m_LastValidatedTime = time;
			player.ApplySnapshotState(corrected);
			return true;
		}
		// Loop to find the relevant state in history
		while (m_PredictedMoveResults[m_Start].Time < time) {
			RemoveOldest();
			// If emptied history, corrected state is in the future. Should just snap to it.
			// This may happen if your client is on pause/breakpoint
			if (m_Count == 0) {
				m_LastValidatedTime = time;
				player.ApplySnapshotState(corrected);
				LOG_WARN("Predicted move time is ahead of corrected state time. Delta: {0}", time - m_PredictedMoveResults[(m_Start - 1) % Net::PREDICTED_STATES].Time);
				return true;
			}
		}
		m_LastValidatedTime = m_PredictedMoveResults[m_Start].Time; // Rounding validation time up to next state time

		const float sqrPosDeltaThreshold = 0.2f;
		float sqrPosDelta = glm::length2(m_PredictedMoveResults[m_Start].State.Position - corrected.Position);
		if (sqrPosDelta > sqrPosDeltaThreshold) {
			LOG("Correcting state at Time {0} | Pos Sqrd Difference: {1} | Delta Time: {2}",
				time, sqrPosDelta, time - m_PredictedMoveResults[m_Start].Time);
			m_PredictedMoveResults[m_Start].State = corrected;
			// Update player to corrected state. [buffer + latency] ms in the past
			player.ApplySnapshotState(corrected);
			// Update all states after with the correction
			for (int i = 1; i < m_Count; ++i) {
				int index = (m_Start + i) % Net::PREDICTED_STATES;
				PredictedMove& move = m_PredictedMoves[index];
				player.ProcessInput(move.InputState, move.DeltaTime);
				m_PredictedMoveResults[index].State = PlayerSnapshotState(player);
			}
			ASSERT((m_Start + m_Count) % Net::PREDICTED_STATES == m_End, "Count does not end on m_End");
			return true;
		}
		return false;
	}
}