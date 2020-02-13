#include "SkelPCH.h"
#include "ClientSynchronizer.h"
#include "ClientManager.h"
#include <FakeLag/FakeLagPacketHolderManager.h>
#include "Game/GameManager.h"

namespace Skel::Net {
	// Start synchronizing
	void ClientSynchronizer::StartSynchronizing()
	{
		m_ToBeSentIndex = 0;
		m_Synchronized = false;
		SendSyncPacket();
	}
	// Constrain synchronization within time limit.
	// Will typically send out a new SyncRequest upon receive ServerTimePacket
	// But this function will forget about a sent packet and move on to the next one
	// This will deem the packet as "lost", the packet may still come back as we are waiting for other packets however
	void ClientSynchronizer::UpdateUntilSynchronized()
	{
		if (m_Synchronized) return;

		const double patienceTimeSpan = 1.00; // seconds
		double sent = LastSentTime();
		if (Game.RunningTime() - sent > patienceTimeSpan)
		{
			// End Synchronization Due to Time Out
			if (m_ToBeSentIndex >= SYNC_SAMPLES) {
				ConcludeSynchronization();
				return;
			}

			// Otherwise, send a new packet
			SendSyncPacket();
		}
		// Using fake lag here and in sendSyncPacket();
		Buffer buffer;
		FakeLagPackets.PopAndSendToServer<SyncRequestPacket>(buffer);
	}
	void ClientSynchronizer::ReceiveServerTimePacket(SyncServerTimePacket& packet)
	{
		// Record Time
		m_SyncDataSlots[packet.SyncID].ReturnTime = Game.RunningTime();
		m_SyncDataSlots[packet.SyncID].ServerTime = packet.ServerTime;
		
		// End Synchronization Due to Time Out
		if (m_ToBeSentIndex >= SYNC_SAMPLES) {
			ConcludeSynchronization();
			return;
		}

		// Send new sync sample
		SendSyncPacket();
	}

	// Called when you complete synchronizing process
	void ClientSynchronizer::ConcludeSynchronization()
	{
		if (m_Synchronized) return;
		m_Synchronized = true;
		double sumRTT = 0.0;
		uint8 received = 0;
		for (const auto& data : m_SyncDataSlots)
		{
			if (data.Received()) {
				sumRTT += data.RTT();
				++received;
			}
		}
		m_AverageLatency = (sumRTT / double(received)) * 0.5; // Finds average latency from client to server

		// Find average clock offset
		double sumOffset = 0.0;
		for (const auto& data : m_SyncDataSlots)
		{
			if (data.Received()) {
				sumOffset += data.ServerTime - (data.ReturnTime - m_AverageLatency);
			}
		}
		m_AverageOffset = (sumOffset / double(received));

		// Debug Log
		LOG("Average Latency: {0}", m_AverageLatency);
		LOG("Average Offset: {0}", m_AverageOffset);

		for (const auto& data : m_SyncDataSlots)
		{
			if (data.Received()) {
				double latency = data.RTT() * 0.5;
				double offset = data.ServerTime - (data.ReturnTime - m_AverageLatency);
				LOG("Data Point: Latency: {0}   |   Offset: {1}", m_AverageLatency - latency, m_AverageOffset - offset);
			}
			else {
				LOG("Data not received");
			}
		}
		LOG("\n");
	}

	// Retrieves SentTime of last packet to be sent
	double ClientSynchronizer::LastSentTime()
	{
		if (m_ToBeSentIndex <= 0)
			return -1;

		return m_SyncDataSlots[m_ToBeSentIndex - 1].SentTime;
	}

	// Average latency in seconds. Half of RTT
	double ClientSynchronizer::Latency()
	{
		return m_AverageLatency;
	}

	// ServerTime - ClientTime
	double ClientSynchronizer::Offset()
	{
		return m_AverageOffset;
	}

	// Helper function to send packet and record entry
	void ClientSynchronizer::SendSyncPacket()
	{
		if (m_ToBeSentIndex < 0 || m_ToBeSentIndex >= SYNC_SAMPLES) {
			LOG_WARN("Failed to send SyncPacket. SentIndex is: {0}", m_ToBeSentIndex);
			return;
		}
		Buffer buffer; // Creating buffer here since we only use this function a few times
		WRITE_PACKET(SyncRequestPacket, (Client.GetClientID(), m_ToBeSentIndex), buffer); // SyncId is the Index in array
		FakeLagPackets.AddPacket<SyncRequestPacket>(packet);
		FakeLagPackets.PopAndSendToServer<Net::SyncRequestPacket>(buffer);
		//if (/*Client.SendBuffer(buffer)*/ ) 
		{

			// Log Time of Sent Packet
			m_SyncDataSlots[m_ToBeSentIndex].SentTime = Game.RunningTime();

			// Prepare Index for Next Packet
			++m_ToBeSentIndex;
		}
	}
}