#pragma once

#include <string>

template <typename T>
class SharedPtr 
{
private:
	T* m_ptr = nullptr;			// ָ��ʵ����Դ��ָ��
	int* m_count = nullptr;		// ָ�����ü�����ָ��

public:
	SharedPtr(T* obj)
	{
		if (obj != nullptr)
		{
			m_ptr = obj;
			m_count = new int(1);
		}
	}

	SharedPtr(const SharedPtr& other)
	{
		if (other.m_ptr != nullptr)
		{
			++(*other.m_count);
			m_ptr = other.m_ptr;
			m_count = other.m_count;
		}
	}

	SharedPtr& operator=(const SharedPtr& other)
	{
		if (this != &other)
		{
			SharedPtr(other).swap(*this);
		}
		return *this;
	}

	~SharedPtr()
	{
		if (m_ptr != nullptr)
		{
			--(*m_count);
			if (*m_count == 0)
				delete m_ptr;
		}
	}

private:
	void swap(SharedPtr& other)
	{
		std::swap(m_ptr, other.m_ptr);
		std::swap(m_count, other.m_count);
	}
};

void testSharedPtr()
{
	SharedPtr<std::string> ptr = new std::string("abc");
	SharedPtr<std::string> ptr2 = new std::string("def");
	ptr2 = ptr;
}