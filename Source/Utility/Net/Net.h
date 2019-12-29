#pragma once

#include <WinSock2.h>
#include <ws2ipdef.h>
#include <WS2tcpip.h>

namespace Skel::Net
{
	class Address;
	inline constexpr uint16 MAX_PLAYERS = 8;
	inline constexpr uint16 FAKE_LAG_S = 0.4; //seconds
	inline constexpr uint16 SYNC_SAMPLES = 30; // how many sync packets are sent out

	void Init();
	Address GetServerAddress();
}