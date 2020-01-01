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
		PlayerInputPacket(PlayerInputState& input, uint16 clientID, uint64 clientTick, double delta) : 
			Packet(Net::PACKET_INPUT), InputState(input), ClientID(clientID), ClientTick(clientTick), DeltaTime(delta) {}
		PlayerInputState InputState;
		uint16 ClientID;
		uint64 ClientTick;
		double DeltaTime;
	protected:
		// Inner function used to transcribe input
		void WriteToBuffer_Internal(Net::Buffer& buffer) override {
			B_WRITE(ClientID);
			B_WRITE(ClientTick);
			B_WRITE(DeltaTime);

			uint8 state = 0;
			state |= (InputState.Forward ? 1 : 0);
			state |= (InputState.Back    ? 1 : 0) << 1;

			B_WRITE(state);

			ASSERT(buffer.Length() == 20, "Input Packet must be 20 bytes");
		}
		// Inner function used to transcribe input
		void ReadFromBuffer_Internal(Net::Buffer& buffer) override {
			B_READ(ClientID);
			B_READ(ClientTick);
			B_READ(DeltaTime);

			uint8 state;
			B_READ(state);
			InputState.Forward	= state & 1;
			InputState.Back		= state & (1 << 1);

			// 1 (type) + 2 (clientID) + 8 (tick) + 8 (deltaTime) + 1(state)
			ASSERT(buffer.GetReadPosition() == 20, "Input Packet must be 20 bytes"); 
		}
	};
}