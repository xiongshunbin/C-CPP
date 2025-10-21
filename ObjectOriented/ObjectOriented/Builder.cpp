#include <iostream>
#include <vector>
#include <string>
#include <map>

/**
 * 1.���崬��ص���
 * 2.���������������
 * 3.�������� -> ���첻ͬ���Ĵ�
 */

// ����ɣ��ŵĴ�
// �洢�Ѿ�����õ��㲿��
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

// ����÷���ŵĴ�
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

// ������������
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
		m_sunny->addParts("�����������ǵ�");
	}

	void buildWeapon() override
	{
		m_sunny->addParts("ʨ����");
	}

	void buildEngine() override
	{
		m_sunny->addParts("������������ȼ��");
	}

	void buildInterior() override
	{
		m_sunny->addParts("�ǳ�����������װ��");
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
		m_merry->assemble("����ľ��", "����");
	}

	void buildWeapon() override
	{
		m_merry->assemble("��ͨ�����Ŵ���", "����ϵͳ");
	}

	void buildEngine() override
	{
		m_merry->assemble("��ú��������", "����ϵͳ");
	}

	void buildInterior() override
	{
		m_merry->assemble("�ǳ�����������װ��", "����");
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

// ���������
class Director
{
public:
	Director() { }
	~Director() { }
	void setBuilder(ShipBuilder* builder)
	{
		m_builder = builder;
	}

	// ��Լ��
	void buildSimpleShip()
	{
		m_builder->buidBody();
		m_builder->buildEngine();
	}

	// ��׼��
	void buildStandardShip()
	{
		buildSimpleShip();
		m_builder->buildWeapon();
	}

	// ������
	void buildRegalShip()
	{
		buildStandardShip();
		m_builder->buildInterior();
	}

private:
	ShipBuilder* m_builder = nullptr;
};

#if 0

// ���Դ���
// ����ɣ���
void buildSunnyShip()
{
	Director* director = new Director;
	SunnyBuilder* builder = new SunnyBuilder;
	director->setBuilder(builder);

	// ��Լ��
	director->buildSimpleShip();
	SunnyShip* ship = builder->getSunnyShip();
	ship->showParts();
	delete ship;

	// ��׼��
	builder->reset();
	director->buildStandardShip();
	ship = builder->getSunnyShip();
	ship->showParts();
	delete ship;

	// ������
	builder->reset();
	director->buildRegalShip();
	ship = builder->getSunnyShip();
	ship->showParts();
	delete ship;

	delete builder;
	delete director;
}

// ����÷����
void buildMerryShip()
{
	Director* director = new Director;
	MerryBuilder* builder = new MerryBuilder;
	director->setBuilder(builder);

	// ��Լ��
	director->buildSimpleShip();
	MerryShip* ship = builder->getMerryShip();
	ship->showParts();
	delete ship;

	// ��׼��
	builder->reset();
	director->buildStandardShip();
	ship = builder->getMerryShip();
	ship->showParts();
	delete ship;

	// ������
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