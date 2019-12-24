#include "SkelPCH.h"

#include "Packet.h"

namespace Skel::Net 
{
	void Packet::ReadFromBuffer(Buffer& buffer)
	{
		buffer.ResetReadPosition();
		uint8 packetType; 
		buffer.Read(&packetType, 1);
		ASSERT(packetType == m_Type, "Wrong buffer is being read. Type mismatch");

		ReadFromBuffer_Internal(buffer);
	}
	void Packet::WriteToBuffer(Buffer& buffer)
	{
		buffer.ResetLength();
		buffer.Write(&m_Type, sizeof(m_Type)); // First byte is packet type

		WriteToBuffer_Internal(buffer);
	}
}

