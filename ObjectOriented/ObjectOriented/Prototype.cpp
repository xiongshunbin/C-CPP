#include <iostream>
#include <string>

/**
 * 原型模式需要注意对象的深拷贝和浅拷贝
 */

// 父类
class GermaSoldier
{
public:
	GermaSoldier() { }
	virtual GermaSoldier* clone() = 0;
	virtual std::string whoAmI() = 0;
	virtual ~GermaSoldier() { }

private:

};

// 子类1
class Soldier66 : public GermaSoldier
{
public:
	GermaSoldier* clone() override
	{
		return new Soldier66(*this);
	}

	std::string whoAmI() override
	{
		return "I am Soldier66.";
	}
};

// 子类2
class Soldier67 : public GermaSoldier
{
public:
	GermaSoldier* clone() override
	{
		return new Soldier67(*this);
	}

	std::string whoAmI() override
	{
		return "I am Soldier67.";
	}
};

#if 0

int main()
{
	GermaSoldier* obj = new Soldier66;
	GermaSoldier* soldier = obj->clone();
	std::cout << soldier->whoAmI() << std::endl;
	delete soldier;
	delete obj;

	obj = new Soldier67;
	soldier = obj->clone();
	std::cout << soldier->whoAmI() << std::endl;
	delete soldier;
	delete obj;

	return 0;
}

#endif