#include <iostream>
#include <string>

// 抽象机器人类
class AbstractRobot
{
public:
	// 武器
	virtual void weapon() = 0;
	// 外观
	virtual void appearance() = 0;
	// 战斗能力
	virtual void fightAbility() = 0;
	// 名字
	virtual std::string getName() = 0;
	// 自愈能力
	virtual void selfHealing() { }
	// 是否能飞
	virtual bool canFlying()
	{
		return false;
	}
	// 是否可以自动控制
	virtual bool isAuto()
	{
		return true;
	}
	// 得到当前机器人的属性
	virtual void getProperty()
	{
		std::cout << "贝加庞克制造的" << getName() << "有以下属性: " << std::endl;
		if (canFlying())
		{
			std::cout << "有飞行能力..." << std::endl;
		}
		else
		{
			std::cout << "没有飞行能力..." << std::endl;
		}

		if (isAuto())
		{
			std::cout << "可以自动控制, 完全体机器人!" << std::endl;
		}
		else
		{
			std::cout << "不能自动控制, 半自动机器人!" << std::endl;
		}
		weapon();
		appearance();
		fightAbility();
		selfHealing();
	}
};

// 机器人子类1 - 和平主义者
class Pacifist : public AbstractRobot
{
public:
	// 武器
	void weapon() override
	{
		std::cout << "可以发射镭射激光..." << std::endl;
	}
	// 外观
	void appearance() override
	{
		std::cout << "外部和巴索罗米·熊一样, 体型庞大, 拥有呈半圆形的耳朵, 内部是金属。" << std::endl;
	}
	// 战斗能力
	void fightAbility() override
	{
		std::cout << "结实抗揍, 可以通过手部或者嘴部发射镭射激光, 可以融化钢铁!!!" << std::endl;
	}
	// 名字
	std::string getName() override
	{
		return "和平主义者";
	}
};

// 机器人子类2 - 炽天使
class Angel : public AbstractRobot
{
public:
	// 武器
	void weapon() override
	{
		std::cout << "可以发射镭射激光, 鹰眼外形的炽天使携带着一把巨剑, 可以斩断一切!!!" << std::endl;
	}
	// 外观
	void appearance() override
	{
		std::cout << "外观和七武海小时候的外形一样, 并且拥有一对和烬一样的翅膀!!!" << std::endl;
	}
	// 战斗能力
	void fightAbility() override
	{
		std::cout << "不仅可以发射镭射激光, 还拥有七武海的能力, 牛逼plus, 无敌了!!!!" << std::endl;
	}
	// 名字
	std::string getName() override
	{
		return "炽天使";
	}
	// 自愈能力
	void selfHealing() override
	{
		std::cout << "非常厚实抗揍, 并且拥有非常强的自愈能力, 开挂了!!!" << std::endl;
	}
	// 是否能飞
	bool canFlying() override
	{
		return true;
	}
};

#if 0

int main()
{
	AbstractRobot* robot = new Pacifist;
	robot->getProperty();
	delete robot;

	std::cout << "------------------------------" << std::endl;

	robot = new Angel;
	robot->getProperty();
	delete robot;
	return 0;
}

#endif
