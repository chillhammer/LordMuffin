#include "SkelPCH.h"
#include <Net/Net.h>
#include <Server/ServerManager.h>
#include "SnapshotPacket.h"

namespace Skel::Net {
	// Creates packet and takes snapshot of world state
	PlayerSnapshotPacket::PlayerSnapshotPacket(const ClientHandler& handler, uint16 clientID) : Packet(Net::PACKET_SNAPSHOT)
	{
		ClientTickNumber = handler.GetClientTick(clientID);
		TickNumber = Server.GetTick();
		Timestamp = Server.RunningTime();
		ServerTakeSnapshot(handler);
	}
	void PlayerSnapshotPacket::ServerTakeSnapshot(const ClientHandler& handler)
	{
#ifndef SERVER
		ASSERT(false, "Cannot Take Snapshot if not on server");
#endif
		uint16 activePlayers = handler.ActivePlayers();
		m_Entries.clear();
		m_Entries.reserve(activePlayers);
		for (uint16 i = 0, clientNum = 0; i < MAX_PLAYERS, clientNum < activePlayers; ++i) {
			if (handler.IsActive(i)) {
				++clientNum;

				const PlayerComponent* obj = &((handler.GetPlayerObject(i))->GetComponent<PlayerComponent>());
				ASSERT(obj != nullptr, "Only enter objects that are active");

				PlayerSnapshotState state(*obj, handler.GetMovedByServerThisFrame(i));

				m_Entries.emplace_back(i, state);
			}
		}
		ASSERT(m_Entries.size() == activePlayers, "Entries incorrect size");
	}
}