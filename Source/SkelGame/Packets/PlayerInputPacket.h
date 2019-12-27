#pragma once
#include <Net/Packet.h>
#include <Objects/Player/PlayerInputState.h>
#include "PacketTypes.h"
#include <Logger/Log.h>

namespace Skel::Net
{
	// From Client
	class PlayerInputPacket : public Net::Packet
	{
	public:
		PlayerInputPacket() : Net::Packet(Net::PACKET_INPUT) {}
		PlayerInputPacket(PlayerInputState& input, uint16 clientID) : 
			Packet(Net::PACKET_INPUT), InputState(input), ClientID(clientID) {}
		PlayerInputState InputState;
		uint16 ClientID;
	protected:
		// Inner function used to transcribe input
		void WriteToBuffer_Internal(Net::Buffer& buffer) override {
			buffer.Write(&ClientID, sizeof(ClientID));

			uint8 state = 0;
			state |= (InputState.Forward ? 1 : 0);
			state |= (InputState.Back    ? 1 : 0) << 1;

			buffer.Write(&state, sizeof(state));
			ASSERT(buffer.Length() == 4, "Input Packet must be 4 bytes");
		}
		// Inner function used to transcribe input
		void ReadFromBuffer_Internal(Net::Buffer& buffer) override {
			buffer.Read(&ClientID, sizeof(ClientID));
			uint8 state;
			buffer.Read(&state, 1);
			//LOG("PlayerInputPacket state: {0}", state);
			InputState.Forward	= state & 1;
			InputState.Back		= state & (1 << 1);
			ASSERT(buffer.GetReadPosition() == 4, "Input Packet must be 4 bytes"); // 1 (type) + 2 (clientID) + 1(state)
		}
	};
}