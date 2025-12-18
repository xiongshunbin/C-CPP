#pragma once

#include <iostream>
#include <string>
#include <list>
#include "Observer.h"

// 发布者基类
class NewsAgency
{
public:
	// 添加订阅者
	void attach(Observer* ob)
	{
		if (ob != nullptr)
		{
			m_list.push_back(ob);
		}
	}

	// 删除订阅者
	void detach(Observer* ob)
	{
		m_list.remove(ob);
	}

	// 通知订阅者
	virtual void notify(std::string msg) = 0;
	virtual ~NewsAgency(){ }

protected:
	std::list<Observer*> m_list;
};

// 发布者子类 - 摩根斯新闻社
class Morgans : public NewsAgency
{
public:
	void notify(std::string msg) override
	{
		std::cout << "摩根斯新闻社报纸的订阅者一共有<" << m_list.size() << ">人" << std::endl;
		for (const auto& item : m_list)
		{
			// 通过观察者更新数据
			item->update(msg);
		}
	}
};

// 发布者子类 - 八卦新闻社
class Gossip : public NewsAgency
{
public:
	// 通知订阅者
	void notify(std::string msg) override
	{
		std::cout << "八卦新闻社报纸的订阅者一共有<" << m_list.size() << ">人" << std::endl;
		for (const auto& item : m_list)
		{
			// 通过观察者更新数据
			item->update(msg);
		}
	}
};
