#include "MyList.h"
#include "Iterator.h"
#include <vector>

Node* MyList::insert(Node* node, std::string name)
{
	if (node == m_head)
	{
		return pushFront(name);
	}
	
	Node* item = new Node(name);
	// 1. 将item和链表中的节点进行相连
	item->next = node;
	item->prev = node->prev;
	// 2. 将原来的连接断开, 重新进行连接
	node->prev->next = item;
	node->prev = item;
	m_count++;
	return item;
}

Node* MyList::pushFront(std::string name)
{
	Node* node = new Node(name);
	if (m_head == nullptr)
	{
		m_head = m_tail = node;
	}
	else
	{
		node->next = m_head;
		m_head->prev = node;
		m_head = node;
	}
	m_count++;
	return node;
}

Node* MyList::pushBack(std::string name)
{
	Node* node = new Node(name);
	if (m_tail == nullptr)
	{
		m_tail = m_head = node;
	}
	else
	{
		node->prev = m_tail;
		m_tail->next = node;
		m_tail = node;
	}
	m_count++;
	return node;
}

std::shared_ptr<Iterator> MyList::getIterator(bool isReverse)
{
	std::shared_ptr<Iterator> it = nullptr;
	if (isReverse)
	{
		it = std::make_shared<ReverseIterator>(this);
	}
	else
	{
		it = std::make_shared<ForwardIterator>(this);
	}
	return it;
}

#if 0

int main()
{
	std::vector<std::string> nameList{
		"烬", "奎因", "杰克", "福兹·弗", "X·德雷克",
		"黑色玛丽亚", "笹木", "润媞", "佩吉万",
		"一美", "二牙", "三鬼", "四鬼", "五鬼",
		"六鬼", "七鬼", "八茶", "九忍", "十鬼"
	};
	MyList ls;
	for (int i = 0; i < nameList.size(); ++i)
	{
		ls.pushFront(nameList.at(i));
	}

	// 正向遍历
	std::shared_ptr<Iterator> it = ls.getIterator(false);
	std::cout << "凯多正向检阅队伍..." << std::endl;
	for (auto begin = it->first(); !it->isDone(); it->next())
	{
		std::cout << "\t" << it->current()->name << "say: 为老大服务!!!" << std::endl;
	}

	std::cout << "--------------------------------------" << std::endl;

	// 逆向遍历
	it = ls.getIterator(true);
	std::cout << "凯多逆向检阅队伍..." << std::endl;
	for (auto begin = it->first(); !it->isDone(); it->next())
	{
		std::cout << "\t" << it->current()->name << "say: 为老大服务!!!" << std::endl;
	}

	return 0;
}

#endif