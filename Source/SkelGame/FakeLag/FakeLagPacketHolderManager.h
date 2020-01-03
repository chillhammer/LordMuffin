#pragma once
#include <Net/FakeLagPacketHolder.h>
#include <Game/GameManager.h>
#include <Server/ServerManager.h>
#include <Packets/SyncPackets.h>
#include <Packets/SnapshotPacket.h>
#include <Packets/PlayerInputPacket.h>

#define FakeLagPackets Net::FakeLagPacketHolderManager::Instance()
namespace Skel::Net
{
	/**
		Mantains all the packet holders for fake lag.
		Uses macros to determine between client and server behavior.
		This class is not meant to be used in final product.
		Simulates fake lag by adding delay to sending packets
	**/
	class FakeLagPacketHolderManager
	{
	public:
		static FakeLagPacketHolderManager& Instance()
		{
			static FakeLagPacketHolderManager instance;
			return instance;
		};

		template <typename P> 
		void AddPacket(P packet, Address address)
		{
			GetHolder<P>()->AddPacket(packet, RUNNING_TIME, address);
		}
		template <typename P> 
		void AddPacket(P packet)
		{
			GetHolder<P>()->AddPacket(packet, RUNNING_TIME);
		}

		template <typename P>
		std::vector<P> PopPackets()
		{
			return GetHolder<P>()->PopPackets(RUNNING_TIME);
		}

		// Shortcut method, this behavior will only be associated with this class from client
		template <typename P>
		void PopAndSendToServer(Buffer& buffer)
		{
			std::vector<P> packets = PopPackets<P>();
			for (P& packet : packets) {
				packet.WriteToBuffer(buffer);
				Client.SendBuffer(buffer);
			}
		}
		// Shortcut method, this behavior will only be associated with this class from server
		template <typename P>
		void PopAndSendToClient(Buffer& buffer, Address& address)
		{
			std::vector<P> packets = PopPackets<P>();
			for (P& packet : packets) {
				packet.WriteToBuffer(buffer);
				Server.GetSocket().SendBuffer(buffer, address);
			}
		}

		// Shortcut method to send to recipients, from server
		template <typename P>
		void PopAndSendToRecipient(Buffer& buffer)
		{
			GetHolder<P>()->PopPackets(RUNNING_TIME, &Server.GetSocket(), &buffer);
		}

		~FakeLagPacketHolderManager() {};
	private:
		FakeLagPacketHolderManager() { };

		// Helper function to return appropriate holder
		template <typename P>
		FakeLagPacketHolder<P>* GetHolder()
		{

			if (typeid(P) == typeid(PlayerInputPacket))
				return (FakeLagPacketHolder<P>*)(&m_InputPackets);
			else if (typeid(P) == typeid(SyncRequestPacket))
				return (FakeLagPacketHolder<P>*)(&m_SyncRequestPackets);
			else if (typeid(P) == typeid(SyncServerTimePacket))
				return (FakeLagPacketHolder<P>*)(&m_SyncTimePackets);
			else if (typeid(P) == typeid(PlayerSnapshotPacket))
				return (FakeLagPacketHolder<P>*)(&m_SnapshotPackets);
			ASSERT(false, "Cannot find appropriate FakeLagPacketHolder");
			return nullptr;
		}

		// Properties
		FakeLagPacketHolder<PlayerInputPacket> m_InputPackets;
		FakeLagPacketHolder<SyncRequestPacket> m_SyncRequestPackets;
		FakeLagPacketHolder<SyncServerTimePacket> m_SyncTimePackets;
		FakeLagPacketHolder<PlayerSnapshotPacket> m_SnapshotPackets;

	};
}