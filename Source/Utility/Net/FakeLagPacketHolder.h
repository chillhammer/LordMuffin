#pragma once
#include "Packet.h"
#include <Random/Random.h>
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
			Address Recipient;

			PacketQueueEntry(P packet, double time) : DelayedPacket(packet), EnteredTime(time), Recipient() {}
			PacketQueueEntry(P packet, double time, Address addr) : DelayedPacket(packet), EnteredTime(time), Recipient(addr) {}
		};

		// Adds packet to queue to be sent at a later time
		void AddPacket(P packet, double time) { m_PacketEntries.emplace(packet, time + Random::Float(FAKE_JITTER_S)); };
		// Adds packet with a target recipient. Useful for sending packets to clients
		void AddPacket(P packet, double time, Address addr) { m_PacketEntries.emplace(packet, time + Random::Float(FAKE_JITTER_S), addr); };
		// Tries to pop off queue and returns list of packets. 
		// If given a sender and buffer, will send to recipients
		std::vector<P> PopPackets(double time, Socket* sender = nullptr, Buffer* buffer = nullptr)
		{
			std::vector<P> popped;
			if (m_PacketEntries.empty())
				return popped;

			// Pop off packets that are beyond lag limit
			PacketQueueEntry front = m_PacketEntries.front();
			while (time - front.EnteredTime > FAKE_LAG_S)
			{
				m_PacketEntries.pop();
				// Send this packet
				if (Random::Roll(1.0f - FAKE_PACKET_LOSS)) {
					popped.push_back(front.DelayedPacket);
					if (sender != nullptr && buffer != nullptr) {
						front.DelayedPacket.WriteToBuffer(*buffer);
						sender->SendBuffer(*buffer, front.Recipient);
					}
				}
				else {
					// DEBUG: Print jump packet
					if (typeid(P) == typeid(PlayerInputPacket)) {

						//PlayerInputPacket& packet = reinterpret_cast<PlayerInputPacket>(front.DelayedPacket);
						PlayerInputPacket* packet = (PlayerInputPacket*)&front.DelayedPacket;	
						if (packet->InputState.Jump) {
							LOG("Jump was missed! Tick: {0}", packet->ClientTick);
						}
					}
				}

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