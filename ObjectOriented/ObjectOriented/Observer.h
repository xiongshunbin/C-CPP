#pragma once

#include <string>

class NewsAgency;

// 抽象的观察者类
class Observer
{
public:
	// 和发布者进行关联 - 可以通过构造函数实现
	Observer(std::string name, NewsAgency* news);
	// 和发布者解除关联 - 取消订阅
	void unsubscribe();
	// 更新信息
	virtual void update(std::string& msg) = 0;
	virtual ~Observer(){ }
protected:
	std::string m_name;
	NewsAgency* m_news = nullptr;
};

// 观察者类子类 - 龙
class Dragon : public Observer
{
public:
	using Observer::Observer;
	// 更新信息
	void update(std::string& msg) override
	{
		std::cout << "@@@路飞的老爸革命军龙收到消息: " << msg << std::endl;
	}
};

// 观察者类子类 - 香克斯
class Shanks : public Observer
{
public:
	using Observer::Observer;
	// 更新信息
	void update(std::string& msg) override
	{
		std::cout << "@@@路飞的引路人红发香克斯收到消息: " << msg << std::endl;
	}
};

// 观察者类子类 - 巴托洛米奥
class Bartolomeo : public Observer
{
public:
	using Observer::Observer;
	// 更新信息
	void update(std::string& msg) override
	{
		std::cout << "@@@路飞的头号粉丝巴托洛米奥收到消息: " << msg << std::endl;
	}
};