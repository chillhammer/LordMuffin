#pragma once
#include "Net.h"
#include "Buffer.h"
#include "Address.h"
#include "Macros.h"

namespace Skel::Net
{
	class Socket
	{
	public:
		Socket();
		void Bind(uint16 port);
		bool SendBuffer(const Buffer& buffer, const Address& address) const;
		bool ReceiveBuffer(Buffer& outBuffer, Address& outAddress) const;
	private:
		uint64 m_Socket;
		uint16 m_Port;
		bool m_Bounded;
	};
}