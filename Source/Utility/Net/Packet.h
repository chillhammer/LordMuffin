#pragma once
#include <Macros.h>
#include "Buffer.h"

namespace Skel::Net
{

	class Packet
	{
	public:
		Packet(uint8 type) : m_Type(type) {}

		void WriteToBuffer(Buffer& buffer);

	protected:
		virtual void WriteToBuffer_Internal(Buffer& buffer) {}
	private:
		uint8 m_Type;

	};
}