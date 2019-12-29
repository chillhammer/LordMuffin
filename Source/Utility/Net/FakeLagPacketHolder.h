#pragma once
#include "Packet.h"
#include <queue>

namespace Skel::Net
{

	// Holds onto packets to simulate fake lag
	template <class P>
	class FakeLagPacketHolder
	{
	public:
		FakeLagPacketHolder() {};

		struct PacketQueueEntry
		{
			P DelayedPacket;
			double EnteredTime;

			PacketQueueEntry(P packet, double time) : DelayedPacket(packet), EnteredTime(time) {}
		};

		void AddPacket(P packet, double time) { m_PacketEntries.emplace(packet, time); };
		std::vector<P> PopPackets(double time)
		{
			std::vector<P> popped;
			if (m_PacketEntries.empty())
				return popped;

			// Pop off packets that are beyond lag limit
			PacketQueueEntry front = m_PacketEntries.front();
			while (time - front.EnteredTime > FAKE_LAG_S)
			{
				m_PacketEntries.pop();
				popped.push_back(front.DelayedPacket);
				if (m_PacketEntries.empty())
					break;

				front = m_PacketEntries.front();
			}
			return popped;
		}
	private:
		std::queue<PacketQueueEntry> m_PacketEntries;
	};
}