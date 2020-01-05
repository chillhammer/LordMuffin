#pragma once
#include <Objects/Player/PlayerSnapshotState.h>
#include <Objects/Player/PlayerSnapshotState.h>
#include <Packets/SnapshotPacket.h>
#include <queue>

namespace Skel::Net
{
	struct SnapshotRecord
	{
		std::vector<SnapshotEntry> Entries;
		double Time;
		// Don't use a const reference here since source Entries may get put off stack
		SnapshotRecord(std::vector<SnapshotEntry> entries, double time) : Entries(entries), Time(time) {}
		SnapshotRecord() : Entries(), Time(0) {}
	};

	// Receives Snapshot from packet and handles effect on clients
	class SnapshotReceiver
	{
	public:
		SnapshotReceiver() = default;
		void ReceiveSnapshotPacket(const PlayerSnapshotPacket& packet);
		void Update();
		void ApplySnapshotState(const PlayerSnapshotState& state, PlayerObject& player);
		void ApplySnapshotEntries(const std::vector<SnapshotEntry>& entries);
		void SetPlayerObjectArray(class PlayerObject* arr) { m_PlayerObjectArray = arr; }
		const std::vector<uint16>& GetActiveClients() const { return m_ActiveClients; }
	private:
		std::vector<SnapshotEntry> InterpolateRecords(const SnapshotRecord& a, const SnapshotRecord& b);
		std::queue<SnapshotRecord> m_ReceivedStates;
		PlayerSnapshotState m_LastReceivedState;
		uint64 m_LastReceivedTick = 0;
		class PlayerObject* m_PlayerObjectArray; // Active/Non-active player objects
		std::vector<uint16> m_ActiveClients;
		bool m_ReceivedFirst = false;
		bool m_Interpolating = false;
		SnapshotRecord m_BaseSnapshot;
		SnapshotRecord m_TargetSnapshot;
	};
}