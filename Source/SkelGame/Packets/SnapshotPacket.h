#pragma once
#include <Net/Packet.h>
#include <Objects/Player/PlayerInputState.h>
#include <Objects/Player/PlayerSnapshotState.h>
#include <Server/ClientHandler.h>
#include "PacketTypes.h"
#include <Logger/Log.h>

namespace Skel::Net
{
	struct SnapshotEntry
	{
		uint16 ClientID;
		PlayerSnapshotState State;

		SnapshotEntry(uint16 id, PlayerSnapshotState state) : ClientID(id), State(std::move(state)) {}
	};

	// From Server
	class PlayerSnapshotPacket : public Net::Packet
	{
	public:
		PlayerSnapshotPacket() : Net::Packet(Net::PACKET_SNAPSHOT) {}
		PlayerSnapshotPacket(const ClientHandler& handler, uint16 clientID); // Only for creation on server

		uint64 ClientTickNumber;	// used for client-side prediction
		uint64 TickNumber;			// server tick
		double Timestamp;
		
		const std::vector<SnapshotEntry>& GetSnapshotEntries() const {
			return m_Entries;
		}

		void ServerTakeSnapshot(const ClientHandler& handler);
	private:
		std::vector<SnapshotEntry> m_Entries;
	protected:
		// Inner function used to transcribe input
		void WriteToBuffer_Internal(Net::Buffer& buffer) override {

			// Tick
			B_WRITE(ClientTickNumber);
			B_WRITE(TickNumber);
			B_WRITE(Timestamp);

			// Num Players1
			uint16 activePlayers = m_Entries.size();
			B_WRITE(activePlayers);

			for (int i = 0; i < activePlayers; ++i)
			{
				SnapshotEntry& entry = m_Entries[i];
				B_WRITE(entry.ClientID);

				B_WRITE(entry.State.Pitch);
				B_WRITE(entry.State.Yaw);
				B_WRITE(entry.State.Position.x);
				B_WRITE(entry.State.Position.y);
				B_WRITE(entry.State.Position.z);
				B_WRITE(entry.State.AnimationIndex);
			}
		}
		// Inner function used to transcribe input
		void ReadFromBuffer_Internal(Net::Buffer& buffer) override {

			// Tick
			B_READ(ClientTickNumber);
			B_READ(TickNumber);
			B_READ(Timestamp);

			// Num Players
			uint16 entriesNum = 0;
			B_READ(entriesNum);

			m_Entries.reserve(entriesNum);
			for (int i = 0; i < entriesNum; ++i) {
				uint16 clientID;
				B_READ(clientID);

				PlayerSnapshotState state;
				B_READ(state.Pitch);
				B_READ(state.Yaw);
				B_READ(state.Position.x);
				B_READ(state.Position.y);
				B_READ(state.Position.z);
				B_READ(state.AnimationIndex);

				m_Entries.emplace_back(clientID, state);
			}
		}
	};
}