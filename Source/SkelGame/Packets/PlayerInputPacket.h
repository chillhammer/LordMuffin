#pragma once
#include <Net/Packet.h>
#include <Objects/Player/PlayerInputState.h>
#include "PacketTypes.h"
#include <Logger/Log.h>
#include <Client/ClientManager.h>

namespace Skel::Net
{
	// From Client
	class PlayerInputPacket : public Net::Packet
	{
	public:
		PlayerInputPacket() : Net::Packet(Net::PACKET_INPUT) {}
		PlayerInputPacket(PlayerInputState& input, uint16 clientID, uint64 clientTick, double delta) : 
			Packet(Net::PACKET_INPUT), InputState(input), ClientID(clientID), ClientTick(clientTick), DeltaTime(delta) {
			// Grabbing Recent Inputs here and not WriteToBuffer because of FakeLag
			RecentInputs = Client.GetPredictionHistory().RecentInputs(Net::INPUTS_PACKED);
		}
		PlayerInputState InputState;
		std::vector<PlayerInputState> RecentInputs;
		uint16 ClientID;
		uint64 ClientTick;
		double DeltaTime;
	protected:
		// Inner function used to transcribe input
		void WriteToBuffer_Internal(Net::Buffer& buffer) override {
			B_WRITE(ClientID);
			B_WRITE(ClientTick);
			B_WRITE(DeltaTime);

			// TODO: refactor input to buffer. finalize input state before refactor
			uint8 state = 0;
			state |= static_cast<uint8>(InputState.Forward);
			state |= static_cast<uint8>(InputState.Back) << 1;
			state |= static_cast<uint8>(InputState.Right) << 2;
			state |= static_cast<uint8>(InputState.Left) << 3;
			state |= static_cast<uint8>(InputState.Jump)	 << 4;

			B_WRITE(state);
			B_WRITE(InputState.Yaw);

			ASSERT(RecentInputs.size() == Net::INPUTS_PACKED, "RecentInputs should be filled in constructor");
			for (const PlayerInputState& recentInput : RecentInputs) {
				state = 0;
				state |= static_cast<uint8>(recentInput.Forward);
				state |= static_cast<uint8>(recentInput.Back) << 1;
				state |= static_cast<uint8>(recentInput.Right) << 2;
				state |= static_cast<uint8>(recentInput.Left) << 3;
				state |= static_cast<uint8>(recentInput.Jump) << 4;
				B_WRITE(state);
				float recentYaw = recentInput.Yaw;
				B_WRITE(recentYaw);
			}

			//ASSERT(buffer.Length() == 20 + Net::INPUTS_PACKED, "Input Packet must be 20 + inputs_packed");
		}
		// Inner function used to transcribe input
		void ReadFromBuffer_Internal(Net::Buffer& buffer) override {
			B_READ(ClientID);
			B_READ(ClientTick);
			B_READ(DeltaTime);

			uint8 state;
			B_READ(state);
			InputState.Forward	= state & BIT(0);
			InputState.Back		= state & BIT(1);
			InputState.Right	= state & BIT(2);
			InputState.Left		= state & BIT(3);
			InputState.Jump		= state & BIT(4);
			B_READ(InputState.Yaw);

			RecentInputs.clear();
			RecentInputs.reserve(Net::INPUTS_PACKED);
			for (int i = 0; i < Net::INPUTS_PACKED; ++i) {
				B_READ(state);

				PlayerInputState input;
				input.Forward = state & BIT(0);
				input.Back = state & BIT(1);
				input.Right = state & BIT(2);
				input.Left = state & BIT(3);
				input.Jump = state & BIT(4);
				B_READ(input.Yaw);
				RecentInputs.push_back(input);
			}

			// 1 (type) + 2 (clientID) + 8 (tick) + 8 (deltaTime) + 1(state) + inputs_packed(states)
			//ASSERT(buffer.GetReadPosition() == 20 + Net::INPUTS_PACKED, "Input Packet must be 20 bytes + inputs_packed"); 
		}
	};
}