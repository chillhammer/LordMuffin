#pragma once
#include <Macros.h>

namespace Skel
{
	template <class N>
	class BitArray
	{
	public:

		BitArray(int size)
		{
			ASSERT(size > 0, "BitArray must have starting size");
			m_Size = size;
			// calculates # of bytes needed by multiples of 8 bytes at a time
			m_Bytes = 8 * ((size / 64) + ((size % 64) ? 1 : 0)); 
			ASSERT(m_Bytes > 0, "Bytes needed must be positive");
			Clear();
		}

		~BitArray() { }

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
			m_Data[data_index] |= BIT(bit_index);
		}

		void ClearBit(int index)
		{
			ASSERT(index >= 0 && index < m_Size, "Index out of bounds");
			const int data_index = index >> 6;
			const int bit_index = index & ((1 << 6) - 1);
			m_Data[data_index] &= ~(BIT(bit_index));
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

	private:
		BitArray(const BitArray& other);
		BitArray& operator = (const BitArray& other);

		int m_Size;
		int m_Bytes;
		uint64_t m_Data[N];

		
	};
}