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

// ���幤����
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