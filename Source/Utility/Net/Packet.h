#pragma once
#include <Macros.h>
#include "Buffer.h"

namespace Skel::Net
{

	enum PacketType : uint8
	{
		PACKET_INPUT = 0,
		CUSTOM_PACKETS
	};

	class Packet
	{
	public:
		Packet(PacketType type) : m_Type(type) {}

		void ReadFromBuffer(Buffer& buffer);
		void WriteToBuffer(Buffer& buffer);
	protected:
		virtual void WriteToBuffer_Internal(Buffer& buffer) {}
		virtual void ReadFromBuffer_Internal(Buffer& buffer) {}
	private:
		PacketType m_Type;

	};
}