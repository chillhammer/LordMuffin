#include "SkelPCH.h"
#include "Net.h"
#include "Address.h"

namespace Skel::Net {

	void Init()
	{
		WSADATA wsaData;

		// Initialize Winsock
		int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (result != 0) {
			LOG_ERROR("WSAStartup failed: %d\n", result);
		}
	}

	Address GetServerAddress()
	{
		return Address(127, 0, 0, 1, 8082);
	}

}