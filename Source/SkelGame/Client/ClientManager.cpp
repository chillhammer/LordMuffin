#include "SkelPCH.h"
#include "ClientManager.h"

namespace Skel::Net {

	ClientManager& ClientManager::Instance()
	{
#ifdef SERVER
		ASSERT(false, "Cannot reference Client singleton from Server");
#endif
		static ClientManager instance;
		return instance;
	}

	bool ClientManager::SendBuffer(const Buffer& buffer)
	{
		Address serverAddress = GetServerAddress();
		
		return m_Socket.SendBuffer(buffer, serverAddress);
	}

	bool ClientManager::ReceiveBuffer(Buffer& outBuffer)
	{
		Address serverAddress = GetServerAddress();

		return m_Socket.ReceiveBuffer(outBuffer, serverAddress);
	}

}