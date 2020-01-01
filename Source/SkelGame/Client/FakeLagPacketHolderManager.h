#pragma once
#include <Net/FakeLagPacketHolder.h>
#include <Game/GameManager.h>
#include <Packets/SyncPackets.h>
#include <Packets/PlayerInputPacket.h>

#define FakeLagPackets Net::FakeLagPacketHolderManager::Instance()
namespace Skel::Net
{
	/**
		Mantains all the packet holders for fake lag.
		Currently only has support from client-side.
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
		void AddPacket(P packet)
		{
			GetHolder<P>()->AddPacket(packet, Game.RunningTime());
		}

		template <typename P>
		std::vector<P> PopPackets()
		{
			return GetHolder<P>()->PopPackets(Game.RunningTime());
		}

		// Shortcut method, this behavior will only be associated with this class
		template <typename P>
		void PopAndSendToServer(Buffer& buffer)
		{
			std::vector<P> packets = PopPackets<P>();
			for (P& packet : packets) {
				packet.WriteToBuffer(buffer);
				Client.SendBuffer(buffer);
			}
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
			ASSERT(false, "Cannot find appropriate FakeLagPacketHolder");
			return nullptr;
		}

		// Properties
		FakeLagPacketHolder<PlayerInputPacket> m_InputPackets;
		FakeLagPacketHolder<SyncRequestPacket> m_SyncRequestPackets;

	};
}