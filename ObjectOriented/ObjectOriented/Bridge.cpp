#include <iostream>
#include <string>
#include <map>

/**
 * 桥接模式: 
 *		抽象层: 被动地受实现层控制;
 *		实现层: 主动的操控抽象层;
 * 思想: 在一个复杂的视线中, 分离出抽象和实现, 通过聚合的方式合并为整体, 协同工作。抽象层和实现层对应的对象的生命周期不受对方影响。
 * 需求: 对海贼世界中的 海贼团 和 海军 通过桥接模式进行管理
 */

// 成员对应的类
class Person
{
public:
	Person(std::string name, std::string job, std::string ability, std::string reward, std::string remarks)
		: m_name(name), m_job(job), m_ability(ability), m_reward(reward), m_remarks(remarks) { }

	std::string name() { return m_name; }
	std::string job() { return m_job; }
	std::string ability() { return m_ability; }
	std::string reward() { return m_reward; }
	std::string remarks() { return m_remarks; }

private:
	std::string m_name;
	std::string m_job;
	std::string m_ability;
	std::string m_reward;
	std::string m_remarks;
};

// 定义实现部分 - 团队
class AbstractTeam
{
public:
	AbstractTeam(std::string name) : m_name(name) { }
	std::string getName() { return m_name; }
	void addMember(Person* person)
	{
		m_teamMap.insert(std::make_pair(person->name(), person));
	}
	void show()
	{
		std::cout << m_name << ": " << std::endl;
		for (const auto& item : m_teamMap)
		{
			std::cout << "【name: " << item.second->name()
				<< ", job: " << item.second->job()
				<< ", ability: " << item.second->ability()
				<< ", reward: " << item.second->reward()
				<< ", remarks: " << item.second->remarks()
				<< "】" << std::endl;
		}
	}
	virtual void executeTask() = 0;
	virtual ~AbstractTeam()
	{
		for (const auto& item : m_teamMap)
		{
			delete item.second;
		}
	}

protected:
	std::string m_name;
	std::map<std::string, Person*> m_teamMap;
};

// 草帽海贼团
class CaoMaoTeam : public AbstractTeam
{
public:
	using AbstractTeam::AbstractTeam;
	void executeTask() override
	{
		std::cout << "在海上冒险, 找到 ONE PIECE 成为海贼王!" << std::endl;
	}
};

// 海军
class SmokerTeam : public AbstractTeam
{
public:
	using AbstractTeam::AbstractTeam;
	void executeTask() override
	{
		std::cout << "为了正义, 先将草帽一网打尽!!!" << std::endl;
	}
};


// 定义抽象部分 - 船
class AbstractShip
{
public:
	AbstractShip(AbstractTeam* team) : m_team(team) { }
	void show()
	{
		m_team->show();
		m_team->executeTask();
	}
	virtual std::string getName() = 0;
	virtual void feature() = 0;

protected:
	AbstractTeam* m_team;
};

class MerryShip : public AbstractShip
{
public:
	using AbstractShip::AbstractShip;
	std::string getName() override
	{
		return "前进·梅利号";
	}
	void feature() override
	{
		std::cout << getName()
			<< " -- 船首为羊头, 化身船精灵在司法岛舍己就下了草帽一伙!" << std::endl;
	}
};

class HaiJunShip : public AbstractShip
{
public:
	using AbstractShip::AbstractShip;
	std::string getName() override
	{
		return "无敌·海军号";
	}
	void feature() override
	{
		std::cout << getName()
			<< " -- 船底由海楼石建造, 可以穿过无风带的巨大炮舰!" << std::endl;
	}
};

#if 0

int main()
{
	AbstractTeam* team = new CaoMaoTeam("草帽海贼团");
	Person* luffy = new Person("路飞", "船长", "橡胶果实能力者", "30亿贝里", "爱吃肉");
	Person* zoro = new Person("索隆", "剑士", "三刀流", "11亿1100万贝里", "路痴");
	Person* sanji = new Person("山治", "厨师", "隐形黑", "10亿3200万贝里", "好色");
	Person* nami = new Person("娜美", "航海士", "天候棒+宙斯", "3亿6600万贝里", "喜欢钱");
	team->addMember(luffy);
	team->addMember(zoro);
	team->addMember(sanji);
	team->addMember(nami);

	AbstractShip* ship = new MerryShip(team);
	ship->feature();
	ship->show();

	delete team;
	delete ship;

	std::cout << "------------------------------------------------------------------------------------" << std::endl;

	team = new SmokerTeam("斯摩格海军部队");
	Person* smoker = new Person("斯摩格", "中将", "冒烟果实能力者", "", "爱吃烟鸡肉");
	Person* dasiqi = new Person("达斯琪", "大佐", "一刀流", "", "近视");
	team->addMember(smoker);
	team->addMember(dasiqi);

	ship = new HaiJunShip(team);
	ship->feature();
	ship->show();

	delete team;
	delete ship;

	return 0;
}

#endif