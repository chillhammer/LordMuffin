#pragma once
#include <Macros.h>

namespace Skel
{
	// Bit Array where N is how many bits you want
	class BitArray
	{
	public:

		BitArray(int size)
		{
			m_Size = size;
			ASSERT(m_Size > 0, "BitArray must have starting size");
			m_Bytes = 8* ((m_Size / 64) + ((m_Size % 64) ? 1 : 0));
			m_Data = new uint64[m_Bytes];
			Clear();
		}

		~BitArray() {
			delete[] m_Data;
		}

		void Clear()
		{
			ASSERT(m_Data, "Data not set");
			memset(m_Data, 0, m_Bytes);
		}

		void SetBit(int index)
		{
			ASSERT(index >= 0 && index < m_Size, "Index out of bounds");
			const int data_index = index >> 6;
			const int bit_index = index & ((1 << 6) - 1);
			ASSERT(bit_index >= 0 && bit_index < 64, "Bit index out of bounds");
			uint64 bit = uint64(1) << index;
			m_Data[data_index] |= bit;
		}

		void ClearBit(int index)
		{
			ASSERT(index >= 0 && index < m_Size, "Index out of bounds");
			const int data_index = index >> 6;
			const int bit_index = index & ((1 << 6) - 1);
			uint64 bit = uint64(1) << index;
			m_Data[data_index] &= ~(bit);
		}

		uint64_t GetBit(int index) const
		{
			ASSERT(index >= 0 && index < m_Size, "Index out of bounds");
			const int data_index = index >> 6;
			const int bit_index = index & ((1 << 6) - 1);
			ASSERT(bit_index >= 0 && bit_index < 64, "Bit index out of bounds");
			return (m_Data[data_index] >> bit_index) & 1;
		}

		int GetSize() const { return m_Size; }
		int GetBytes() const { return m_Bytes; }

	private:
		BitArray(const BitArray& other) {};
		BitArray& operator = (const BitArray& other) {ASSERT(false, "Not supported")};

		int m_Size;
		int m_Bytes;
		uint64_t* m_Data;

		
	};
}