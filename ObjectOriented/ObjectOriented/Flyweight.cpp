#include <iostream>
#include <string>
#include <map>
#include <vector>

/**
 * 核心思想:
 *		1.分离数据块内部的静态资源
 *		2.共享分离出来的静态资源
 *		3.享元工厂类
 */

// 享元类的基类
class FlyweightBody
{
public:
	FlyweightBody(std::string sprite) : m_sprite(sprite) { }
	virtual void move(int x, int y, int speed) = 0;
	virtual void draw(int x, int y) = 0;
	virtual ~FlyweightBody() { }

protected:
	std::string m_sprite;
	std::string m_color;
};

// 享元类的子类 - 炮弹
class SharedBombBody : public FlyweightBody
{
public:
	using FlyweightBody::FlyweightBody;
	void move(int x, int y, int speed) override
	{
		std::cout << "炸弹以每小时" << speed << "的速度飞到了("
			<< x << ", " << y << ")的位置......" << std::endl;
	}

	void draw(int x, int y) override
	{
		std::cout << "在(" << x << ", " << y << ")的位置重绘炸弹......" << std::endl;
	}
};

// 享元类的子类 - 彩蛋
class UniqueBombBody : public FlyweightBody
{
public:
	using FlyweightBody::FlyweightBody;
	void move(int x, int y, int speed) override
	{
		// 此处省略对参数x, y, speed的处理
		std::cout << "彩蛋在往指定位置移动, 准备爆炸发放奖励......" << std::endl;
	}

	void draw(int x, int y) override
	{
		std::cout << "在(" << x << ", " << y << ")的位置重绘彩蛋......" << std::endl;
	}
};

// 发射炮弹
class LaunchBomb
{
public:
	LaunchBomb(FlyweightBody* body) : m_body(body) { }
	void setSpeed(int speed)
	{
		m_speed = speed;
	}

	void move(int x, int y)
	{
		m_x = x;
		m_y = y;
		m_body->move(m_x, m_y, m_speed);
		draw();
	}

	void draw()
	{
		m_body->draw(m_x, m_y);
	}

private:
	FlyweightBody* m_body;
	int m_speed = 100;
	int m_x = 0;
	int m_y = 0;
};

// 享元工厂类
class BombBodyFactory
{
public:
	// 参数就是炸弹的型号
	FlyweightBody* getSharedBomb(std::string name)
	{
		FlyweightBody* bomb = nullptr;
		// 遍历容器
		for (auto& item : m_bodyMap)
		{
			if (item.first == name)
			{
				bomb = item.second;
				std::cout << "正在复用<" << name << ">......" << std::endl;
				break;
			}
		}

		if (bomb == nullptr)
		{
			bomb = new SharedBombBody(name);
			std::cout << "正在创建<" << name << ">......" << std::endl;
			m_bodyMap.insert(std::make_pair(name, bomb));
		}
		return bomb;
	}

	~BombBodyFactory()
	{
		for (auto& item : m_bodyMap)
		{
			delete item.second;
		}
	}

private:
	std::map<std::string, FlyweightBody*> m_bodyMap;
};

#if 0

int main()
{
	BombBodyFactory* factory = new BombBodyFactory;
	std::vector<LaunchBomb*> bombList;
	std::vector<std::string> nameList = { "撒旦-1", "撒旦-1", "撒旦-2", "撒旦-2", "撒旦-2", "撒旦-3", "撒旦-3", "撒旦-3" };
	for (auto& name : nameList)
	{
		int x = 0, y = 0;
		LaunchBomb* launch = new LaunchBomb(factory->getSharedBomb(name));
		for (int i = 0; i < 3; ++i)
		{
			x += rand() % 100;
			y += rand() % 50;
			launch->move(x, y);
		}
		bombList.push_back(launch);
	}
	// 彩蛋
	UniqueBombBody* unique = new UniqueBombBody("大彩蛋");
	LaunchBomb* bomb = new LaunchBomb(unique);
	int x = 0, y = 0;
	for (int i = 0; i < 3; ++i)
	{
		x += rand() % 100;
		y += rand() % 50;
		bomb->move(x, y);
	}

	for (auto& bomb : bombList)
	{
		delete bomb;
	}

	delete factory;
	delete bomb;
	delete unique;
	return 0;
}

#endif