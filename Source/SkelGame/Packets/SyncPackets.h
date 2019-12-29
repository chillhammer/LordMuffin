#pragma once
#include <Net/Packet.h>
#include "PacketTypes.h"

namespace Skel::Net
{
	// From Client
	class SyncRequestPacket : public Net::Packet
	{
	public:
		SyncRequestPacket(uint16 id) : Net::Packet(Net::PACKET_SYNC_REQUEST), ClientID(id) {}
		uint16 ClientID;
		uint16 SyncID;
	protected:
		void WriteToBuffer_Internal(Net::Buffer& buffer) override {
			B_WRITE(ClientID);
			B_WRITE(SyncID);
		}

		void ReadFromBuffer_Internal(Net::Buffer& buffer) override {
			B_READ(ClientID);
			B_READ(SyncID);
		}
	};

	// From Server
	class SyncServerTimePacket : public Net::Packet
	{
	public:
		SyncServerTimePacket() : Net::Packet(Net::PACKET_SYNC_SERVER_TIME) {}
		SyncServerTimePacket(uint16 clientID, uint16 syncId, double time) : 
			Net::Packet(Net::PACKET_JOIN_ACCEPT), ClientID(clientID), SyncID(syncId), ServerTime(time) {}

		uint16 ClientID;
		uint16 SyncID;
		double ServerTime;
	protected:

		void WriteToBuffer_Internal(Net::Buffer& buffer) override {
			B_WRITE(ClientID);
			B_WRITE(SyncID);
			B_WRITE(ServerTime);
		}

		void ReadFromBuffer_Internal(Net::Buffer& buffer) override {
			B_READ(ClientID);
			B_READ(SyncID);
			B_READ(ServerTime);
		}
	};
}