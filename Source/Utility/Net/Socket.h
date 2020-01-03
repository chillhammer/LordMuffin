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
		void SetLogErrors(bool log) { m_LogErrors = log; }
	private:
		uint64 m_Socket = 0;
		uint16 m_Port;
		bool m_Bounded;
		bool m_LogErrors;
	};
}