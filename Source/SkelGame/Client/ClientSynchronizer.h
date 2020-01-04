#pragma once
#include <Packets/SyncPackets.h>
#include <Net/Net.h>

namespace Skel::Net
{
	struct SyncData
	{
		double SentTime = -1;
		double ServerTime = -1;
		double ReturnTime = -1;
		bool Received() const { return ReturnTime > 0; }
		double RTT() const { return ReturnTime - SentTime; }
	};

	// Handles syncing with server from client
	class ClientSynchronizer
	{
	public:
		void StartSynchronizing();
		void UpdateUntilSynchronized();
		void ReceiveServerTimePacket(SyncServerTimePacket& packet);
		bool IsSynchronized() const { return m_Synchronized; }	// Is complete
		bool IsSynchronizing() const { return m_ToBeSentIndex >= 0 && m_ToBeSentIndex < SYNC_SAMPLES || !m_Synchronized; } // Is in process
		double LastSentTime();
		double Latency();
		double Offset();
	private:
		void SendSyncPacket();
		void ConcludeSynchronization();
	private:
		SyncData m_SyncDataSlots[SYNC_SAMPLES];
		uint16 m_ToBeSentIndex = -1;
		bool m_Synchronized = false;
		double m_AverageLatency = -1; // Calculated Upon Sync Completion
		double m_AverageOffset = -1;  // Calculated Upon Sync Completion. Client/Server clock offset
	};
}