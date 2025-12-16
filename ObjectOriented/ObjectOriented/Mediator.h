#pragma once

#include <iostream>
#include <string>
#include <map>

class Country;

// 定义中介者类的基类
class MediatorOrg
{
public:
	void addMember(Country* country);
	virtual void declare(std::string msg, Country* country, std::string name) = 0;
	virtual ~MediatorOrg() { }

protected:
	std::map<std::string, Country*> m_countryMap;
};

// 世界政府
class WorldGovt : public MediatorOrg
{
public:
	void declare(std::string msg, Country* country, std::string name) override;
};

// 革命军
class GeMingArmy : public MediatorOrg
{
public:
	void declare(std::string msg, Country* country, std::string name) override;
};
