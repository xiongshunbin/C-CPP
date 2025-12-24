#include "Visitor.h"
#include "Member.h"
#include <iostream>
#include <list>
#include <vector>
#include <memory>
#include <map>

void Anger::maleDoing(MaleMember* male)
{
	std::cout << "我是草帽海贼团的" << male->getName() << std::endl;
	fight();
}

void Anger::femaleDoing(FemaleMember* female)
{
	std::cout << "我是草帽海贼团的" << female->getName() << std::endl;
	warning();
}

void Anger::fight()
{
	std::cout << "只要还活着就得跟这家伙血战到底!!!" << std::endl;
}

void Anger::warning()
{
	std::cout << "大家快逃, 我快顶不住了, 不用管我!!!" << std::endl;
}

Anger::~Anger()
{
	std::cout << "Anger is sacrificed!" << std::endl;
}

void Horror::maleDoing(MaleMember* male)
{
	std::cout << "我是草帽海贼团的" << male->getName() << std::endl;
	thinking();
}

void Horror::femaleDoing(FemaleMember* female)
{
	std::cout << "我是草帽海贼团的" << female->getName() << std::endl;
	help();
}

void Horror::help()
{
	std::cout << "这个大熊太厉害, 太可怕了, 快救救我。。。" << std::endl;
}

void Horror::thinking()
{
	std::cout << "得辅助同伴们一起攻击这个家伙, 不然根本打不过呀!!!" << std::endl;
}

Horror::~Horror()
{
	std::cout << "Horror is sacrificed!" << std::endl;
}

// 定义一个用于测试的类
class CaoMaoTeam
{
public:
	CaoMaoTeam()
	{
		m_actions.push_back(std::make_shared<Anger>());
		m_actions.push_back(std::make_shared<Horror>());
	}

	~CaoMaoTeam()
	{
		for (const auto& item : m_members)
		{
			delete item;
		}
	}

	void add(AbstractMember* member)
	{
		if (member != nullptr)
		{
			m_members.push_back(member);
		}
	}

	void remove(AbstractMember* member)
	{
		if (member != nullptr)
		{
			m_members.remove(member);
		}
	}

	void display()
	{
		for (const auto& item : m_members)
		{
			int index = rand() % 2;
			item->accept(m_actions.at(index).get());
		}
	}

private:
	std::list<AbstractMember*> m_members;
	std::vector<std::shared_ptr<AbstractAction>> m_actions;	// m_actions[0]: Anger, m_actions[1]: Horror
};

#if 1

int main()
{
	srand(time(NULL));
	std::map<std::string, Sex> members = {
		{"路飞", Sex::Male},
		{"索隆", Sex::Male},
		{"山治", Sex::Male},
		{"乔巴", Sex::Male},
		{"弗兰奇", Sex::Male},
		{"乌索普", Sex::Male},
		{"布鲁克", Sex::Male},
		{"娜美", Sex::Female},
		{"罗宾", Sex::Female}
	};

	CaoMaoTeam* team = new CaoMaoTeam;
	for (const auto& item : members)
	{
		Sex sex = item.second;
		if (sex== Sex::Male)
		{
			team->add(new MaleMember(item.first));
		}
		else if (sex == Sex::Female)
		{
			team->add(new FemaleMember(item.first));
		}
	}

	team->display();

	delete team;
	return 0;
}

#endif
