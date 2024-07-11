#include <iostream>

using namespace std;

// ��Ʒ�ĸ���
class AbstractSmile
{
public:
	// ֻ������͸�����ڼ̳й�ϵ��������Ҫʵ�ֶ�̬�ĳ����²���ҪΪ�����ṩ����������
	// ���ܱ�֤������Ķ�����������ʱ�������ȷ�����������ͷ���Դ
	virtual ~AbstractSmile() { }
	virtual void transform() = 0;
	virtual void ability() = 0;
};


class SheepSmile : public AbstractSmile
{
public:
	void transform() override
	{
		cout << "������� -- ɽ������̬..." << endl;
	}
	void ability() override
	{
		cout << "���ֱ۱������ǵ���ʽ -- �����" << endl;
	}
};

class LionSmile : public AbstractSmile
{
public:
	void transform() override
	{
		cout << "������� -- ʨ������̬..." << endl;
	}
	void ability() override
	{
		cout << "��� �� ������֮��..." << endl;
	}
};

class BatSmile : public AbstractSmile
{
public:
	void transform() override
	{
		cout << "������� -- ��������̬..." << endl;
	}
	void ability() override
	{
		cout << "��������֮�򽣹���..." << endl;
	}
};

enum class Type :char { Sheep, Lion, Bat };

// ���幤���� - ����
class AbstractFactory
{
public:
	virtual AbstractSmile* createSmile() = 0;
	virtual ~AbstractFactory() { } 
};


// ����ɽ��Ķ�ħ��ʵ
class SheepFactory : public AbstractFactory
{
public:
	AbstractSmile* createSmile() override
	{
		return new SheepSmile;
	}
	~SheepFactory()
	{
		cout << "SheepFactory ��������..." << endl;
	}
};

// ����ʨ�ӵĶ�ħ��ʵ
class LionFactory : public AbstractFactory
{
public:
	AbstractSmile* createSmile() override
	{
		return new LionSmile;
	}
	~LionFactory()
	{
		cout << "LionFactory ��������..." << endl;
	}
};

// ��������Ķ�ħ��ʵ
class BatFactory : public AbstractFactory
{
public:
	AbstractSmile* createSmile() override
	{
		return new BatSmile;
	}
	~BatFactory()
	{
		cout << "BatFactory ��������..." << endl;
	}
};


#if 0
int main()
{
	AbstractFactory* factory = new LionFactory;
	AbstractSmile* obj = factory->createSmile();
	obj->transform();
	obj->ability();

	delete factory;
	delete obj;

	return 0;
}
#endif