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
		PlayerInputPacket(PlayerInputState& input) : Packet(Net::PACKET_INPUT), InputState(input) {}
		PlayerInputState InputState;
	protected:
		// Inner function used to transcribe input
		void WriteToBuffer_Internal(Net::Buffer& buffer) override {
			uint8 state = 0;
			state |= (InputState.Forward ? 1 : 0);
			state |= (InputState.Back    ? 1 : 0) << 1;

			buffer.Write(&state, sizeof(state));
			ASSERT(buffer.Length() == 2, "Input Packet must be 2 bytes");
		}
		// Inner function used to transcribe input
		void ReadFromBuffer_Internal(Net::Buffer& buffer) override {
			uint8 state;
			buffer.Read(&state, 1);
			//LOG("PlayerInputPacket state: {0}", state);
			InputState.Forward	= state & 1;
			InputState.Back		= state & (1 << 1);
			ASSERT(buffer.GetReadPosition() == 2, "Input Packet must be 2 bytes");
		}
	};
}