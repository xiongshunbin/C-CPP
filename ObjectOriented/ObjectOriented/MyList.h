#pragma once

#include <string>
#include <memory>

// 类声明
class Iterator;

// 定义一个节点
struct Node
{
	Node(std::string str) : name(str) { }
	std::string name = std::string();
	Node* next = nullptr;
	Node* prev = nullptr;
};

// 定义双向链表
class MyList
{
public:
	inline int getCount()
	{
		return m_count;
	}

	inline Node* head()
	{
		return m_head;
	}

	inline Node* tail()
	{
		return m_tail;
	}

	Node* insert(Node* node, std::string name);
	Node* pushFront(std::string name);
	Node* pushBack(std::string name);
	std::shared_ptr<Iterator> getIterator(bool isReverse = false);

private:
	Node* m_head = nullptr;
	Node* m_tail = nullptr;
	int m_count = 0;
};

