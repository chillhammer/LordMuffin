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
	bool PlayerPredictionStateHistory::CorrectState(const PlayerSnapshotState& corrected, PlayerObject& player, double time, uint64 clientTick)
	{
		uint64 ticksBehind = Game.GetTick() - clientTick;

		m_LastValidatedTime = time; // debug purposes

		// Exit out if same tick. Rare edge case. Impossible in high latency
		if (ticksBehind == 0) return false;

		// If nothing to compare to, snap to 'corrected' state 
		if (m_Count == 0 || (ticksBehind > m_Count) ) {
			LOG_WARN("Correcting client state. Cannot find appropriate record");
			player.ApplySnapshotState(corrected);
			
			return true;
		}
		// Loop to find the relevant state in history. The state to compare corrected to
		// Target State should be the state that the action occurred
		uint64 targetStart = (m_End - ticksBehind - 1) % Net::PREDICTED_STATES;
		while (m_Start != targetStart) {

			RemoveOldest();
			// If emptied history, corrected state is in the future. Should just snap to it.
			// This may happen if your client is on pause/breakpoint
			if (m_Count == 0) {
				player.ApplySnapshotState(corrected);
				LOG_WARN("Predicted move time is ahead of corrected state time. Delta: {0}", time - m_PredictedMoveResults[(m_Start - 1) % Net::PREDICTED_STATES].Time);
				return true;
			}
		}
		
		// The state in which the action occured
		PredictedMoveResult relevant = m_PredictedMoveResults[m_Start];
		const float sqrPosDeltaThreshold = 0.2f;
		float sqrPosDelta = glm::length2(relevant.State.Position - corrected.Position);

		// Check next state also
		PredictedMoveResult relevant2 = m_PredictedMoveResults[(m_Start + 1) % Net::PREDICTED_STATES];
		float sqrPosDelta2 = glm::length2(relevant2.State.Position - corrected.Position);
		
		if (sqrPosDelta > sqrPosDeltaThreshold && sqrPosDelta2 > sqrPosDeltaThreshold) {
			LOG("Correcting state at Time {0} | Pos Sqrd Difference: {1} | Delta Time: {2}",
				time, sqrPosDelta, time - relevant.Time);

			// Update player to corrected state. [buffer + latency] ms in the past
			player.ApplySnapshotState(corrected);

			// Update all states after with the correction
			for (int i = 0; i < m_Count; ++i) {
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
	// Returns the n newest states (excluding newest)
	std::vector<PlayerInputState> PlayerPredictionStateHistory::RecentInputs(uint8 num) const
	{
		std::vector<PlayerInputState> recent;
		recent.reserve(num);
		
		int index = ((m_End - 2) + Net::PREDICTED_STATES) % Net::PREDICTED_STATES; // State before newest
		for (int i = 0; i < num; i++) {
			ASSERT(index >= 0, "index must be non-negative")
			recent.push_back(m_PredictedMoves[index].InputState);
			index = ((index - 1) + Net::PREDICTED_STATES )% Net::PREDICTED_STATES;
		}

		return recent;
	}
}