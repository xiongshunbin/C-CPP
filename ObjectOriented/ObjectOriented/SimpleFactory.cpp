#include <iostream>

using namespace std;

// 产品的父类
class AbstractSmile
{
public:
	// 只有子类和父类存在继承关系，并且需要实现多态的场景下才需要为父类提供虚析构函数
	// 才能保证在子类的对象在析构的时候调用正确的析构函数释放资源
	virtual ~AbstractSmile() { }
	virtual void transform() = 0;
	virtual void ability() = 0;
};


class SheepSmile : public AbstractSmile
{
public:
	void transform() override
	{
		cout << "变成人兽 -- 山羊人形态..." << endl;
	}
	void ability() override
	{
		cout << "将手臂变成绵羊角的招式 -- 巨羊角" << endl;
	}
};

class LionSmile : public AbstractSmile
{
public:
	void transform() override
	{
		cout << "变成人兽 -- 狮子人形态..." << endl;
	}
	void ability() override
	{
		cout << "火遁 · 豪火球之术..." << endl;
	}
};

class BatSmile : public AbstractSmile
{
public:
	void transform() override
	{
		cout << "变成人兽 -- 蝙蝠人形态..." << endl;
	}
	void ability() override
	{
		cout << "声纳引箭之万剑归宗..." << endl;
	}
};

enum class Type :char { Sheep, Lion, Bat };

// 定义工厂类
class SmileFactory
{
public:
	AbstractSmile* createSmile(Type type)
	{
		AbstractSmile* ptr = nullptr;
		switch (type)
		{
		case Type::Sheep:
			ptr = new SheepSmile;
			break;
		case Type::Lion:
			ptr = new LionSmile;
			break;
		case Type::Bat:
			ptr = new BatSmile;
			break;
		default:
			break;
		}
		return ptr;
	}
};

#if 0

int main()
{
	SmileFactory* factory = new SmileFactory;
	AbstractSmile* obj = factory->createSmile(Type::Lion);
	obj->transform();
	obj->ability();

	delete factory;
	delete obj;

	return 0;
}

#endif