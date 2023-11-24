#pragma once
#include <algorithm>

template <typename F>
class FVector
{
public:
	FVector() : m_data{ nullptr }, m_size{ 0 }, m_capacity{ 0 }
	{}
	FVector(const FVector<F>& other)
	{
		m_capacity = other.m_capacity;
		m_size = other.m_size;
		m_data = new F[m_capacity];
		for (int i = 0; i < m_size; i++)
		{
			m_data[i] = other.m_data[i];
		}
	}
	~FVector()
	{
		if (m_data)
		{
			delete[] m_data;
		}
	}
	size_t size() const
	{
		return m_size;
	}
	size_t capacity() const
	{
		return m_capacity;
	}
	void push(const F& item)
	{
		if (m_size + 1 > m_capacity)
		{
			IncreaseCapacity();
		}
		m_data[m_size++] = item;
	}
	void insert(const F& item, int index)
	{
		EnsureCapacity(index);
		m_size = std::max(index + 1, (int)m_size + 1);
		m_data[index] = item;
	}
	void remove(int index)
	{
		assert(index < m_size);
		ShifDown(index);
		m_size--;
	}
	void clear()
	{
		m_size = 0;
	}
	F& operator [](int i)
	{
		return m_data[i];
	}
	const F& operator [](int i) const
	{
		return m_data[i];
	}
	FVector<F>& operator=(const FVector<F>& other)
	{
		if (m_data)
		{
			delete[] m_data;
		}

		m_capacity = other.m_capacity;
		m_size = other.m_size;
		m_data = new F[m_capacity];
		for (int i = 0; i < m_size; i++)
		{
			m_data[i] = other.m_data[i];
		}

		return *this;
	}

	// Iterator methods
	F* begin()
	{
		return &m_data[0];
	}

	F* end()
	{
		return &m_data[m_size];
	}

	const F* cbegin() const
	{
		return &m_data[0];
	}

	const F* cend() const
	{
		return &m_data[m_size];
	}
	
private:

	void ShiftUp(int gapIndex)
	{
		for (int index = m_size; index > gapIndex; index--)
		{
			m_data[index] = m_data[index - 1];
		}
	}

	void ShifDown(int deleteIndex)
	{
		for (int index = deleteIndex; index < m_size - 1; index++)
		{
			m_data[index] = m_data[index + 1];
		}
	}

	void IncreaseCapacity(int minCapacity = -1)
	{
		int newCapacity;
		if (minCapacity > 0)
		{
			newCapacity = pow(2, ceil(log2(minCapacity)));
			if (newCapacity <= m_capacity)
			{
				return;
			}
		}
		else
		{
			newCapacity = std::max(1, (int)m_capacity * 2);
		}

		if (m_capacity == 0)
		{
			// If capacity is 0 create create new array on heap
			m_data = new F[newCapacity];
			m_capacity = newCapacity;
		}
		else
		{
			// If capacity is not 0 create new array the heap to store new data
			F* newData = new F[newCapacity];
			
			for (size_t i = 0; i < m_size; ++i)
			{
				new (&newData[i]) F(m_data[i]);  // Placement new and copy constructor
			}

			delete[] m_data;
			m_data = newData;
			m_capacity = newCapacity;
		}
	}

	// Ensures there is capacity for an element to be inserted at the gapIndex. Does not increase m_size even if it changes
	void EnsureCapacity(int gapIndex)
	{
		if (m_capacity > gapIndex && m_size < m_capacity)
		{
			if (m_size > gapIndex)
			{
				ShiftUp(gapIndex);
			}
		}
		else
		{
			// Increase capacity leaving a gap for the new element
			if (m_size > gapIndex)
			{
				int newCapacity = std::max(1, (int)m_capacity * 2);
				if (m_capacity == 0)
				{
					// If capacity is 0 create create new array on heap
					m_data = new F[newCapacity];
					m_capacity = newCapacity;
				}
				else
				{
					// If capacity is not 0 create new array double the size on the heap to store new data
					F* newData = new F[newCapacity];

					for (size_t i = 0; i < gapIndex; ++i)
					{
						new (&newData[i]) F(m_data[i]);  // Placement new and copy constructor
					}
					for (size_t i = gapIndex; i < m_size; ++i)
					{
						new (&newData[i + 1]) F(m_data[i]);  // Placement new and copy constructor
					}

					delete[] m_data;
					m_data = newData;
					m_capacity = newCapacity;
				}
			}
			else
			{
				IncreaseCapacity(gapIndex + 1);
			}
		}
	}

	size_t m_size;
	size_t m_capacity;
	F* m_data;
};

