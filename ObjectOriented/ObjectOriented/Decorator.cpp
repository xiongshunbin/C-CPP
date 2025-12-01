#include <iostream>

// 战士的基类
class Soldier
{
public:
	explicit Soldier(std::string name = "") : m_name(name) { }
	std::string getName() const
	{
		return m_name;
	}
	virtual void fight() = 0;
	virtual ~Soldier() { }

protected:
	std::string m_name = std::string();

};

// 黑胡子
class Teach : public Soldier
{
public:
	using Soldier::Soldier;
	void fight()
	{
		std::cout << m_name << "依靠惊人的力量和高超的体术战斗..." << std::endl;
	}
};

// 恶魔果实基类
class DevilFruit : public Soldier
{
public:
	void enchantment(Soldier* soldier)
	{
		m_soldier = soldier;
		m_name = soldier->getName();
	}
	virtual ~DevilFruit() { }

protected:
	Soldier* m_soldier = nullptr;
};

// 暗暗果实
class DarkFruit : public DevilFruit
{
public:
	void fight() override
	{
		m_soldier->fight();
		// 使用恶魔果实能力战斗
		std::cout << m_soldier->getName()
			<< "吃了暗暗果实, 可以拥有黑洞一样的无限吸引力......" << std::endl;
		warning();
	}
private:
	void warning()
	{
		std::cout << m_soldier->getName()
			<< "注意: 吃了暗暗果实, 身体元素化之后不能躲避攻击, 会吸收所有伤害!" << std::endl;
	}
};

// 震震果实
class QuakeFruit : public DevilFruit
{
public:
	void fight() override
	{
		m_soldier->fight();
		// 使用恶魔果实能力战斗
		std::cout << m_soldier->getName()
			<< "吃了震震果实, 可以在任意空间引发震动, 摧毁目标......" << std::endl;
	}
};

// 大饼果实
class PieFruit : public DevilFruit
{
public:
	void fight() override
	{
		m_soldier->fight();
		// 使用恶魔果实能力战斗
		std::cout << m_soldier->getName()
			<< "吃了大饼果实, 可以获得大丙铠甲......" << std::endl;
		ability();
	}

	void ability()
	{
		std::cout << "最强辅助 -- 大饼果实可以将身边事物变成大饼, 帮助队友回血!" << std::endl;
	}
};

#if 0

int main()
{
	Soldier* soldier = new Teach("黑胡子");
	DevilFruit* dark = new DarkFruit;
	DevilFruit* quake = new QuakeFruit;
	DevilFruit* pie = new PieFruit;

	// 黑胡子吃下暗暗恶魔果实(通过暗暗恶魔果实装饰黑胡子)
	dark->enchantment(soldier);
	// 黑胡子吃下震震恶魔果实(通过震震恶魔果实装饰黑胡子)
	quake->enchantment(dark);
	// 黑胡子吃下大饼恶魔果实(通过大饼恶魔果实装饰黑胡子)
	pie->enchantment(quake);
	pie->fight();

	delete pie;
	delete quake;
	delete dark;
	delete soldier;

	return 0;
}

#endif