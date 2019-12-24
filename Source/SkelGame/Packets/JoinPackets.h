#pragma once
#include <Net/Packet.h>
#include "PacketTypes.h"

namespace Skel::Net
{
	// From Client
	class JoinRequestPacket : public Net::Packet
	{
	public:
		JoinRequestPacket() : Net::Packet(Net::PACKET_JOIN_REQUEST) {}
	};

	// From Server
	class JoinAcceptPacket : public Net::Packet
	{
	public:
		JoinAcceptPacket() : Net::Packet(Net::PACKET_JOIN_ACCEPT) {}
		JoinAcceptPacket(uint16 clientID) : Net::Packet(Net::PACKET_JOIN_ACCEPT), ClientID(clientID) {}

		uint16 ClientID;
	protected:

		void WriteToBuffer_Internal(Net::Buffer& buffer) override {
			buffer.Write(&ClientID, sizeof(ClientID));
		}

		void ReadFromBuffer_Internal(Net::Buffer& buffer) override {
			buffer.Read(&ClientID, sizeof(ClientID));
		}
	};

	// From Server
	class JoinDeclinedPacket : public Net::Packet
	{
	public:
		JoinDeclinedPacket() : Net::Packet(Net::PACKET_JOIN_DECLINED) {}
	};
}