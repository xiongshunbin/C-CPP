#include <iostream>
#include <string>
#include <map>

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
	void move(int x, int y, int speed)
	{
		std::cout << "炸弹以每小时" << speed << "的速度飞到了("
			<< x << ", " << y << ")的位置......" << std::endl;
	}

	void draw(int x, int y)
	{
		std::cout << "在(" << x << ", " << y << ")的位置重绘炸弹......" << std::endl;
	}
};

// 享元类的子类 - 彩蛋
class UniqueBombBody : FlyweightBody
{
public:
	using FlyweightBody::FlyweightBody;
	void move(int x, int y, int speed)
	{
		// 此处省略对参数x, y, speed的处理
		std::cout << "彩蛋在往指定位置移动, 准备爆炸发放奖励......" << std::endl;
	}

	void draw(int x, int y)
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

#if 1

int main()
{

	return 0;
}

#endif