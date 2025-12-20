#include <iostream>

// 抽象的策略类
class AbstractStrategy
{
public:
	virtual void fight(bool isfar = false) = 0;
	virtual ~AbstractStrategy() { }
};

// 一档
class YiDang : public AbstractStrategy
{
public:
	void fight(bool isfar = false) override
	{
		std::cout << "*** 现在使用的是一档: ";
		if (isfar)
		{
			std::cout << "橡胶机关枪" << std::endl;
		}
		else
		{
			std::cout << "橡胶·攻城炮" << std::endl;
		}
	}
};

// 二档
class ErDang : public AbstractStrategy
{
public:
	void fight(bool isfar = false) override
	{
		std::cout << "*** 现在使用的是二档: ";
		if (isfar)
		{
			std::cout << "橡胶Jet火箭" << std::endl;
		}
		else
		{
			std::cout << "橡胶Jet·铳乱打" << std::endl;
		}
	}
};

// 三档
class SanDang : public AbstractStrategy
{
public:
	void fight(bool isfar = false) override
	{
		std::cout << "*** 现在使用的是三档: ";
		if (isfar)
		{
			std::cout << "橡胶巨人回旋弹" << std::endl;
		}
		else
		{
			std::cout << "橡胶巨人战斧" << std::endl;
		}
	}
};

// 四档
class SiDang : public AbstractStrategy
{
public:
	void fight(bool isfar = false) override
	{
		std::cout << "*** 现在使用的是四档: ";
		if (isfar)
		{
			std::cout << "橡胶狮子火箭炮" << std::endl;
		}
		else
		{
			std::cout << "橡胶犀牛榴弹炮" << std::endl;
		}
	}
};

// 五档
class WuDang : public AbstractStrategy
{
public:
	void fight(bool isfar = false) override
	{
		std::cout << "*** 切换成五档: 变成尼卡形态可以把物体变成橡胶, 并任意改变物体的形态对其进行攻击!!!" << std::endl;
	}
};

enum class Level : char 
{
	Easy,
	Normal,
	Hard,
	Experts,
	Professional
};

// 路飞
class Luffy
{
public:
	void fight(Level level, bool isfar = false)
	{
		if (m_strategy != nullptr)
		{
			delete m_strategy;
			m_strategy = nullptr;
		}

		switch (level)
		{
		case Level::Easy:
			m_strategy = new YiDang;
			break;
		case Level::Normal:
			m_strategy = new ErDang;
			break;
		case Level::Hard:
			m_strategy = new SanDang;
			break;
		case Level::Experts:
			m_strategy = new SiDang;
			break;
		case Level::Professional:
			m_strategy = new WuDang;
			break;
		default:
			break;
		}
		m_strategy->fight(isfar);
	}

	~Luffy()
	{
		if (m_strategy != nullptr)
		{
			delete m_strategy;
		}
	}

private:
	AbstractStrategy* m_strategy = nullptr;
};

#if 0

int main()
{
	Luffy* luffy = new Luffy;
	std::cout << "--- 在香波地群岛遇到了海军士兵: " << std::endl;
	luffy->fight(Level::Easy);
	std::cout << "--- 在魔谷镇遇到了贝拉米: " << std::endl;
	luffy->fight(Level::Normal);
	std::cout << "--- 在司法岛遇到了罗布·路奇: " << std::endl;
	luffy->fight(Level::Hard);
	std::cout << "--- 在德雷斯罗萨遇到了多弗朗明哥: " << std::endl;
	luffy->fight(Level::Experts);
	std::cout << "--- 在鬼岛遇到了凯多: " << std::endl;
	luffy->fight(Level::Professional);

	delete luffy;
	return 0;
}

#endif
