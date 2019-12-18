#pragma once

#include <WinSock2.h>
#include <ws2ipdef.h>
#include <WS2tcpip.h>

namespace Skel::Net
{
	class Address;

	void Init();
	Address GetServerAddress();
}