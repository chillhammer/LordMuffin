#pragma once
#include <Net/Socket.h>
#include "SnapshotReceiver.h"

#define Client Net::ClientManager::Instance()
namespace Skel::Net
{
	/**
		Manager singleton object representing client socket and connection to server
		Can only be used by the client
	**/
	class ClientManager
	{
	public:
		static ClientManager& Instance();

		bool Connected() const { return m_Connected; }
		uint16 GetClientID() const { return m_ClientID; }
		void SetClientID(uint16 id) { m_ClientID = id; }
		bool SendBuffer(const Buffer& buffer);
		bool ReceiveBuffer(Buffer& outBuffer);

		void SetConnected(bool connected);

		SnapshotReceiver& GetSnapshotReceiver() { return m_SnapshotReceiver; }

		~ClientManager() {};
	private:
		ClientManager() { m_Socket.SetLogErrors(false); };

		// Properties
		SnapshotReceiver m_SnapshotReceiver;
		Socket m_Socket;
		uint16 m_ClientID;
		bool m_Connected;
		
	};
}