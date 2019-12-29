#pragma once
#include <Packets/SyncPackets.h>

namespace Skel::Net
{
	struct SyncData
	{
		double SentTime = -1;
		double ReturnTime = -1;
		bool Received() const { return ReturnTime > 0; }
	};

	// Handles syncing with server from client
	class ClientSynchronizer
	{
	public:

	private:
		SyncData m_SyncDataSlots[SYNC_SAMPLES];
		uint16 m_ToBeSentIndex = 0;
	};
}