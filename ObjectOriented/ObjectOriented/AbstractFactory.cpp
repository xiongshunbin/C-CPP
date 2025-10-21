#include <iostream>
#include <string.h>

using namespace std;

// ����
class ShipBody
{
public:
	virtual string getBody() = 0;
	virtual ~ShipBody() { }
};

// ľͷ����
class WoodBody : public ShipBody
{
	string getBody() override
	{
		return string("ʹ��<ľ��>�����������Ĵ���...");
	}
};

// ��������
class IronBody : public ShipBody
{
	string getBody() override
	{
		return string("ʹ��<�ֲ�>�����������Ĵ���...");
	}
};

// �ϳɽ�������
class MetalBody : public ShipBody
{
	string getBody() override
	{
		return string("ʹ��<�ϳɽ���>�����������Ĵ���...");
	}
};

// ����
class Engine
{
public:
	virtual string getEngine() = 0;
	virtual ~Engine() { }
};

class Human : public Engine
{
public:
	string getEngine()override
	{
		return "�������Ķ�����ʽ��<�ֶ�>...";
	}
};

class Diesel : public Engine
{
public:
	string getEngine()override
	{
		return "�������Ķ�����ʽ��<��ȼ��>...";
	}
};

class Nuclear : public Engine
{
public:
	string getEngine()override
	{
		return "�������Ķ�����ʽ��<�˷�Ӧ��>...";
	}
};

// ����
class Weapon
{
public:
	virtual string getWeapon() = 0;
	virtual ~Weapon(){ }
};

class Gun : public Weapon
{
	string getWeapon() override
	{
		return string("���ϵ�����ϵͳ�� < ǹ > ...");
	}
};

class Cannon : public Weapon
{
	string getWeapon() override
	{
		return string("���ϵ�����ϵͳ�� < ��ũ�� > ...");
	}
};


class Laser : public Weapon
{
	string getWeapon() override
	{
		return string("���ϵ�����ϵͳ�� < ���� > ...");
	}
};

// ��
class Ship
{
public:
	Ship(ShipBody* body, Engine* engine, Weapon* weapon) :
		m_body(body), m_engine(engine), m_weapon(weapon) { }

	~Ship()
	{
		delete m_body;
		delete m_engine;
		delete m_weapon;
	}
	string getProperty()
	{
		string info = m_body->getBody() + m_weapon->getWeapon() + m_engine->getEngine();
		return info;
	}
private:
	ShipBody* m_body;
	Engine* m_engine;
	Weapon* m_weapon;
};

// ������ - ����
class AbstractFactory
{
public:
	virtual Ship* createShip() = 0;
	virtual ~AbstractFactory() { }

};

class BasicFactory : public AbstractFactory
{
public:
	Ship* createShip() override
	{
		Ship* ship = new Ship(new WoodBody, new Human, new Gun);
		cout << "<�����͵ĺ�����> �Ѿ��������!!!" << endl;
		return ship;
	}
};

class StandardFactory : public AbstractFactory
{
public:
	Ship* createShip() override
	{
		Ship* ship = new Ship(new IronBody, new Diesel, new Cannon);
		cout << "<��׼�͵ĺ�����> �Ѿ��������!!!" << endl;
		return ship;
	}
};

class UltimataFactory : public AbstractFactory
{
public:
	Ship* createShip() override
	{
		Ship* ship = new Ship(new MetalBody, new Nuclear, new Laser);
		cout << "<�콢�͵ĺ�����> �Ѿ��������!!!" << endl;
		return ship;
	}
};

#if 0

int main()
{
	// �µ�Ϊ�콢�ͺ�����
	AbstractFactory* factory = new UltimataFactory;
	Ship* ship = factory->createShip();
	cout << ship->getProperty() << endl;

	delete ship;
	delete factory;

	return 0;
}

#endif