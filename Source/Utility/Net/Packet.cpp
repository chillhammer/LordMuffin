#include "SkelPCH.h"

#include "Packet.h"

namespace Skel::Net 
{
	void Packet::WriteToBuffer(Buffer& buffer)
	{
		ASSERT(buffer.Length() == 0, "Buffer length must be set to 0");
		buffer.Write(&m_Type, sizeof(m_Type));

		WriteToBuffer_Internal();
	}
}

