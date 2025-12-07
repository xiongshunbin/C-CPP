#include <iostream>

enum class RequestType : char
{
	QingJia,
	ZhangXin,
	CiZhi
};

// 抽象的管理者节点
class AbstractManager
{
public:
	void setNext(AbstractManager* next)
	{
		m_next = next;
	}

	virtual void handleRequest(RequestType type) = 0;
	virtual ~AbstractManager() { }

protected:
	AbstractManager* m_next = nullptr;
};

// 底层管理者
class Manager : public AbstractManager
{
public:
	void handleRequest(RequestType type) override
	{
		switch (type)
		{
		case RequestType::QingJia:
			std::cout << "请假: 同意请假, 好好休息~~~" << std::endl;
			break;
		case RequestType::ZhangXin:
			std::cout << "涨薪: 这个我得请示一下咱们的CEO~~~" << " ====> ";
			m_next->handleRequest(type);
			break;
		case RequestType::CiZhi:
			std::cout << "辞职: 我给你向上级反映一下~~~" << " ====> ";
			m_next->handleRequest(type);
			break;
		default:
			break;
		}
	}
};

// CEO
class CEO : public AbstractManager
{
public:
	void handleRequest(RequestType type) override
	{
		switch (type)
		{
		case RequestType::QingJia:
			std::cout << "请假: 同意请假, 下不为例..." << std::endl;
			break;
		case RequestType::ZhangXin:
			std::cout << "涨薪: 你的工资不少了, 给你个购物券吧..." << std::endl;
			break;
		case RequestType::CiZhi:
			std::cout << "辞职: 这个我得问问咱们老板..." << " ====> ";
			m_next->handleRequest(type);
			break;
		default:
			break;
		}
	}
};

// BOSS
class Boss : public AbstractManager
{
public:
	void handleRequest(RequestType type) override
	{
		switch (type)
		{
		case RequestType::QingJia:
			std::cout << "请假: 只有工作才能实现人生价值, 回去好好坚守岗位!!!" << std::endl;
			break;
		case RequestType::ZhangXin:
			std::cout << "涨薪: 钱财乃身外之物, 要视其如粪土!!!" << std::endl;
			break;
		case RequestType::CiZhi:
			std::cout << "辞职: 巴洛克工作社就是你的家, 这次把你留下, 下次别提了!!!" << std::endl;
			break;
		default:
			break;
		}
	}
};

// 提交请求的类
class DaCongMing
{
public:
	void request(RequestType type, AbstractManager* manager)
	{
		manager->handleRequest(type);
	}
};

#if 0

int main()
{
	Manager* manager = new Manager;
	CEO* ceo = new CEO;
	Boss* boss = new Boss;
	manager->setNext(ceo);
	ceo->setNext(boss);

	// 创建员工的实例
	DaCongMing* boy = new DaCongMing;
	std::cout << "大聪明直接将请求提交给顶头上司" << std::endl;
	boy->request(RequestType::QingJia, manager);
	boy->request(RequestType::ZhangXin, manager);
	boy->request(RequestType::CiZhi, manager);

	std::cout << "大聪明直接将请求提交给CEO" << std::endl;
	boy->request(RequestType::QingJia, ceo);
	boy->request(RequestType::ZhangXin, ceo);
	boy->request(RequestType::CiZhi, ceo);

	std::cout << "大聪明直接将请求提交给BOSS" << std::endl;
	boy->request(RequestType::QingJia, boss);
	boy->request(RequestType::ZhangXin, boss);
	boy->request(RequestType::CiZhi, boss);

	delete boy;
	delete manager;
	delete ceo;
	delete boss;

	return 0;
}

#endif
