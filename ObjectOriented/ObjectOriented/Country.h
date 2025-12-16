#pragma once

#include <iostream>
#include "Mediator.h"

// 定义国家类的基类
class Country
{
public:
	explicit Country(MediatorOrg* org = nullptr) : m_mediatorOrg(org) { }
	void setMediator(MediatorOrg* org)
	{
		m_mediatorOrg = org;
	}

	virtual void declare(std::string msg, std::string country = std::string()) = 0;
	virtual void setMessage(std::string msg) = 0;
	virtual std::string getName() = 0;
	virtual ~Country() { }

protected:
	MediatorOrg* m_mediatorOrg = nullptr;
};

// 阿拉巴斯坦
class Alabasta : public Country
{
public:
	using Country::Country;
	void declare(std::string msg, std::string country = std::string()) override
	{
		std::cout << "阿拉巴斯坦向" << country << "喊话: " << std::endl;
		m_mediatorOrg->declare(msg, this, country);
	}

	void setMessage(std::string msg) override
	{
		std::cout << "阿拉巴斯坦收到的消息: " << msg << std::endl;
	}

	std::string getName() override
	{
		return "阿拉巴斯坦";
	}
};

// 德雷斯罗萨
class Dressrosa : public Country
{
public:
	using Country::Country;
	void declare(std::string msg, std::string country = std::string()) override
	{
		std::cout << "德雷斯罗萨发布消息称: " << std::endl;
		m_mediatorOrg->declare(msg, this, country);
	}

	void setMessage(std::string msg) override
	{
		std::cout << "德雷斯罗萨收到的消息: " << msg << std::endl;
	}

	std::string getName() override
	{
		return "德雷斯罗萨";
	}
};

// 露露西亚王国
class Lulusia : public Country
{
public:
	using Country::Country;
	void declare(std::string msg, std::string country = std::string()) override
	{
		std::cout << "露露西亚王国发布公告: " << std::endl;
		m_mediatorOrg->declare(msg, this, country);
	}

	void setMessage(std::string msg) override
	{
		std::cout << "露露西亚王国收到的消息: " << msg << std::endl;
	}

	std::string getName() override
	{
		return "露露西亚王国";
	}
};

// 卡玛巴卡王国
class Kamabaka : public Country
{
public:
	using Country::Country;
	void declare(std::string msg, std::string country = std::string()) override
	{
		std::cout << "卡玛巴卡王国发布公告: " << std::endl;
		m_mediatorOrg->declare(msg, this, country);
	}

	void setMessage(std::string msg) override
	{
		std::cout << "卡玛巴卡王国收到的消息: " << msg << std::endl;
	}

	std::string getName() override
	{
		return "卡玛巴卡王国";
	}
};
