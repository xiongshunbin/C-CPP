#include <iostream>
#include <vector>
#include <string>
#include <map>

/**
 * 1.定义船相关的类
 * 2.定义生成器相关类
 * 3.管理者类 -> 建造不同规格的船
 */

// 定义桑尼号的船
// 存储已经建造好的零部件
class SunnyShip
{
public:
	void addParts(std::string name)
	{
		m_parts.push_back(name);
	}

	void showParts()
	{
		for (const auto& item : m_parts)
		{
			std::cout << item << " ";
		}
		std::cout << std::endl;
	}

private:
	std::vector<std::string> m_parts;
};

// 定义梅利号的船
class MerryShip
{
public:
	void assemble(std::string name, std::string parts)
	{
		m_parts.insert(std::make_pair(parts, name));
	}

	void showParts()
	{
		for (const auto& item : m_parts)
		{
			std::cout << item.first << ":" << item.second << " ";
		}
		std::cout << std::endl;
	}

private:
	std::map<std::string, std::string> m_parts;
};

// 定义生成器类
class ShipBuilder
{
public:
	ShipBuilder() { }
	virtual void reset() = 0;
	virtual void buidBody() = 0;
	virtual void buildWeapon() = 0;
	virtual void buildEngine() = 0;
	virtual void buildInterior() = 0;
	virtual ~ShipBuilder() { }

private:

};

class SunnyBuilder : public ShipBuilder
{
public:
	SunnyBuilder() 
	{
		reset();
	}

	void reset() override
	{
		m_sunny = new SunnyShip;
	}

	SunnyShip* getSunnyShip()
	{
		SunnyShip* ship = m_sunny;
		m_sunny = nullptr;
		return ship;
	}

	void buidBody() override
	{
		m_sunny->addParts("船体是神树亚当");
	}

	void buildWeapon() override
	{
		m_sunny->addParts("狮吼炮");
	}

	void buildEngine() override
	{
		m_sunny->addParts("可乐驱动的内燃机");
	}

	void buildInterior() override
	{
		m_sunny->addParts("非常豪华的内室装修");
	}

	~SunnyBuilder() 
	{
		if (m_sunny != nullptr)
		{
			delete m_sunny;
		}
	}

private:
	SunnyShip* m_sunny = nullptr;
};


class MerryBuilder : public ShipBuilder
{
public:
	MerryBuilder()
	{
		reset();
	}

	void reset() override
	{
		m_merry = new MerryShip;
	}
	
	MerryShip* getMerryShip()
	{
		MerryShip* ship = m_merry;
		m_merry = nullptr;
		return ship;
	}

	void buidBody() override
	{
		m_merry->assemble("优质木材", "船体");
	}

	void buildWeapon() override
	{
		m_merry->assemble("普通的四门大炮", "武器系统");
	}

	void buildEngine() override
	{
		m_merry->assemble("烧煤的蒸汽机", "动力系统");
	}

	void buildInterior() override
	{
		m_merry->assemble("非常豪华的内室装修", "内室");
	}

	~MerryBuilder()
	{
		if (m_merry != nullptr)
		{
			delete m_merry;
		}
	}

private:
	MerryShip* m_merry = nullptr;
};

// 定义管理者
class Director
{
public:
	Director() { }
	~Director() { }
	void setBuilder(ShipBuilder* builder)
	{
		m_builder = builder;
	}

	// 简约型
	void buildSimpleShip()
	{
		m_builder->buidBody();
		m_builder->buildEngine();
	}

	// 标准型
	void buildStandardShip()
	{
		buildSimpleShip();
		m_builder->buildWeapon();
	}

	// 豪华型
	void buildRegalShip()
	{
		buildStandardShip();
		m_builder->buildInterior();
	}

private:
	ShipBuilder* m_builder = nullptr;
};

#if 0

// 测试代码
// 建造桑尼号
void buildSunnyShip()
{
	Director* director = new Director;
	SunnyBuilder* builder = new SunnyBuilder;
	director->setBuilder(builder);

	// 简约型
	director->buildSimpleShip();
	SunnyShip* ship = builder->getSunnyShip();
	ship->showParts();
	delete ship;

	// 标准型
	builder->reset();
	director->buildStandardShip();
	ship = builder->getSunnyShip();
	ship->showParts();
	delete ship;

	// 豪华型
	builder->reset();
	director->buildRegalShip();
	ship = builder->getSunnyShip();
	ship->showParts();
	delete ship;

	delete builder;
	delete director;
}

// 降噪梅利号
void buildMerryShip()
{
	Director* director = new Director;
	MerryBuilder* builder = new MerryBuilder;
	director->setBuilder(builder);

	// 简约型
	director->buildSimpleShip();
	MerryShip* ship = builder->getMerryShip();
	ship->showParts();
	delete ship;

	// 标准型
	builder->reset();
	director->buildStandardShip();
	ship = builder->getMerryShip();
	ship->showParts();
	delete ship;

	// 豪华型
	builder->reset();
	director->buildRegalShip();
	ship = builder->getMerryShip();
	ship->showParts();
	delete ship;

	delete builder;
	delete director;
}

int main()
{
	buildMerryShip();
	std::cout << "------------------------------" << std::endl;
	buildSunnyShip();
	
	return 0;
}

#endif