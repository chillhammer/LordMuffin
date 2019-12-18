#pragma once

namespace Skel::Net
{
	// Wrapper to hold data. Send and recieve this data over the internet
	class Buffer
	{
	public:
		friend class Socket;

		static const int MaxSize = 1024;

		Buffer() : m_Data(), m_Length(0) {}

		Buffer(void* data, unsigned int size) : m_Data(), m_Length(0) {
			Write(data, size);
		}

		const char* Data() const {
			return m_Data;
		}
		
		unsigned int Length() const {
			return m_Length;
		};

		void Write(void* data, unsigned int size) {
			ASSERT(m_Length + size < MaxSize, "Cannot to write to buffer. Overflow");
			memcpy(m_Data, data, size);
			m_Length += size;
		}

	private:
		char m_Data[MaxSize];
		unsigned int m_Length;
	};
}