#pragma once

#include <WinSock2.h>
#include <ws2ipdef.h>
#include <WS2tcpip.h>

namespace Skel::Net
{
	class Address;
	inline constexpr uint16 MAX_PLAYERS = 8;

	void Init();
	Address GetServerAddress();
}