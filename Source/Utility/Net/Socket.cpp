#include "SkelPCH.h"
#include "Socket.h"

namespace Skel::Net {
	Socket::Socket()
	{
		m_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (m_Socket == INVALID_SOCKET)
		{
			LOG_ERROR("socket failed: {0}", WSAGetLastError());
			return;
		}
	}

	// Binding to a socket. Useful for servers that need to have a known port. 
	// Clients do not need to Bind
	void Socket::Bind(uint16 port)
	{
		SOCKADDR_IN local_address;
		local_address.sin_family = AF_INET;
		local_address.sin_port = htons(port);
		local_address.sin_addr.s_addr = INADDR_ANY;
		if (bind(m_Socket, (SOCKADDR*)& local_address, sizeof(local_address)) == SOCKET_ERROR)
		{
			LOG_ERROR("bind failed: {0}", WSAGetLastError());
			return;
		}
		m_Bounded = true;
		m_Port = port;
	}

	// Send information
	bool Socket::SendBuffer(const Buffer& buffer, const Address& address) const
	{
		const sockaddr* toAddress = nullptr;
		ASSERT(address.GetType() != ADDRESS_UNDEFINED, "Address is undefined");
		switch (address.GetType()) {
		case ADDRESS_IPV4:	toAddress = (const sockaddr*) & (address.GetSockAddr4()); break;
		case ADDRESS_IPV6:	toAddress = (const sockaddr*) & (address.GetSockAddr6()); break;
		}
		
		int result = sendto(m_Socket, buffer.Data(), buffer.MaxSize, 0, toAddress, sizeof(*toAddress)); // change from maxsize
		if (result == SOCKET_ERROR) {
			LOG_ERROR("send buffer failed: {0}", WSAGetLastError());
		}
		return result == buffer.Length();
	}

	// Retrieve information from any address
	// Allows direct access to Buffer due to Socket being a friend class
	bool Socket::ReceiveBuffer(Buffer& outBuffer, Address& outAddress) const
	{

		sockaddr_storage from;
		socklen_t fromLength = sizeof(from);

		int result = recvfrom(m_Socket, (char*)outBuffer.m_Data, outBuffer.MaxSize, 0, (sockaddr*)& from, &fromLength);

		if (result <= 0)
		{
			if (result == 0)
				LOG("cannot receive buffer. connection closed");
			else
				LOG_ERROR("receive buffer failed: {0}\n", strerror(errno));
			return false;
		}
		outBuffer.m_Length += result;

		outAddress = Address(from);
		return result;
	}

}