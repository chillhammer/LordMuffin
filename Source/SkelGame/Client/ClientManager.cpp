#include "SkelPCH.h"
#include <EventSystem/Events/NetEvent.h>
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

	void ClientManager::SetConnected(bool connected)
	{
		ASSERT(m_Connected != connected, "Cannot set connected to already connected");
		ASSERT(m_ClientID >= 0, "Must have clientID")
		m_Connected = connected;
		m_Socket.SetLogErrors(connected);

		if (connected)
		{
			ClientConnectEvent e(m_ClientID);
			ClientSubject.Notify(e);
		}
		else
		{
			ClientDisconnectEvent e(m_ClientID);
			ClientSubject.Notify(e);
		}
	}

	void ClientManager::SetLastReceivedTime(double time)
	{
		m_LastReceivedTime = time;
	}
	double ClientManager::GetLastReceivedTime() const
	{
		return m_LastReceivedTime;
	}

}