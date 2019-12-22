#pragma once
#include <Net/Socket.h>

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
		bool SendBuffer(const Buffer& buffer);
		bool ReceiveBuffer(Buffer& outBuffer);

		~ClientManager() {};
	private:
		ClientManager() {};

		// Properties
		Socket m_Socket;
		uint16 m_ClientID;
		bool m_Connected;
		
	};
}