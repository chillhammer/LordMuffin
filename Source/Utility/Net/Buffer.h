#pragma once

namespace Skel::Net
{
	// Wrapper to hold data. Send and recieve this data over the internet
	class Buffer
	{
	public:
		friend class Socket;

		static const int MaxSize = 1024;

		Buffer() : m_Data(), m_Length(0), m_ReadPos(0) {}

		Buffer(void* data, unsigned int size) : m_Data(), m_Length(0) {
			Write(data, size);
		}

		const char* Data() const { return m_Data; }
		
		unsigned int Length() const { return m_Length; }

		unsigned int GetReadPosition() const { return m_ReadPos; }

		void ResetLength() { m_Length = 0; }
		void ResetReadPosition() { m_ReadPos = 0; }

		void Write(void* data, unsigned int size) {
			ASSERT(m_Length + size < MaxSize, "Cannot to write to buffer. Overflow");
			ASSERT(size > 0, "Must write positive amount of bytes");
			memcpy(m_Data + m_Length, data, size);
			m_Length += size;
		}

		void Read(void* data, unsigned int size) {
			ASSERT(m_ReadPos + size <= m_Length, "Cannot read past buffer size");
			ASSERT(size > 0, "Must read positive amount of bytes");
			memcpy(data, m_Data + m_ReadPos, size);
			m_ReadPos += size;
		}

	private:
		char m_Data[MaxSize];
		unsigned int m_Length;
		unsigned int m_ReadPos;
	};
}