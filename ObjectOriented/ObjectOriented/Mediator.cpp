#include <iostream>
#include <string>
#include <map>

class MediatorOrg;
// 定义国家类的基类
class Country
{
public:
	Country(MediatorOrg* org) { m_mediatorOrg = org; }
	virtual void declare(std::string msg, std::string country) = 0;
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
	virtual void declare(std::string msg, std::string country)
	{
		std::cout << "德雷斯罗萨向" << country << "喊话: " << std::endl;
		m_mediatorOrg->declare(msg, this, country);
	}

	virtual void setMessage(std::string msg)
	{
		std::cout << "阿拉巴斯坦收到的消息: " << msg << std::endl;
	}

	virtual std::string getName()
	{
		return "阿拉巴斯坦";
	}
};

// 德雷斯罗萨
class Dressrosa : public Country
{
public:
	using Country::Country;
	virtual void declare(std::string msg, std::string country)
	{
		std::cout << "德雷斯罗萨开始发布公告: " << std::endl;
		m_mediatorOrg->declare(msg, this, country);
	}

	virtual void setMessage(std::string msg)
	{
		std::cout << "德雷斯罗萨收到的消息: " << msg << std::endl;
	}

	virtual std::string getName()
	{
		return "德雷斯罗萨";
	}
};

// 露露西亚王国
class Lulusia : public Country
{
public:
	using Country::Country;
	virtual void declare(std::string msg, std::string country)
	{
		std::cout << "露露西亚王国开始发布公告: " << std::endl;
		m_mediatorOrg->declare(msg, this, country);
	}

	virtual void setMessage(std::string msg)
	{
		std::cout << "露露西亚王国收到的消息: " << msg << std::endl;
	}

	virtual std::string getName()
	{
		return "露露西亚王国";
	}
};

// 卡玛巴卡王国
class Kamabaka : public Country
{
public:
	using Country::Country;
	virtual void declare(std::string msg, std::string country)
	{
		std::cout << "卡玛巴卡王国发布公告: " << std::endl;
		m_mediatorOrg->declare(msg, this, country);
	}

	virtual void setMessage(std::string msg)
	{
		std::cout << "卡玛巴卡王国收到的消息: " << msg << std::endl;
	}

	virtual std::string getName()
	{
		return "卡玛巴卡王国";
	}
};

// 定义中介者类的基类
class MediatorOrg
{
public:
	void addMember(Country* country)
	{
		if (country != nullptr)
		{
			m_countryMap.insert(std::make_pair(country->getName(), country));
		}
	}

	virtual void declare(std::string msg, Country* country, std::string name) = 0;
	virtual ~MediatorOrg() { }

protected:
	std::map<std::string, Country*> m_countryMap;
};

// 世界政府
class WorldGovt : public MediatorOrg
{
public:
	void declare(std::string msg, Country* country, std::string name) override
	{
		if (m_countryMap.find(name) != m_countryMap.end())
		{
			std::string  str = msg + "【来自: " + country->getName() + "】";
			m_countryMap[name]->setMessage(str);
		}
	}
};

// 革命军
class GeMingArmy : public MediatorOrg
{
public:
	void declare(std::string msg, Country* country, std::string name) override
	{
		std::string  str = msg + "【来自: " + country->getName() + "】";
		for (const auto& item : m_countryMap)
		{
			if (item.second == country)
			{
				continue;
			}
			item.second->setMessage(str);
		}
	}
};

#if 1

int main()
{
	// 世界政府
	WorldGovt* world = new WorldGovt;
	// 加盟国对象
	Alabasta* alaba = new Alabasta(world);
	Dressrosa* dress = new Dressrosa(world);
	world->addMember(alaba);
	world->addMember(dress);


	return 0;
}

#endif