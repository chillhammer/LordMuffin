#pragma once
#include <Net/Packet.h>
#include <Objects/Player/PlayerInputState.h>

namespace Skel
{
	using namespace Net;

	class PlayerInputPacket : public Packet
	{
	public:
		PlayerInputPacket() : Packet(0) {}
		PlayerInputPacket(PlayerInputState& input) : Packet(0), Input(input) {}
		PlayerInputState Input;
	protected:
		void WriteToBuffer_Internal(Buffer& buffer) override {
			uint8 state = 0;
			state |= (Input.Forward ? 1 : 0);
			state |= (Input.Back    ? 1 : 0) << 1;

			buffer.Write(&state, sizeof(state));
			ASSERT(buffer.Length() == 2, "Input Packet must be 2 bytes");
		}
	};
}