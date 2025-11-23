#include <iostream>
#include <string>
#include <list>
#include <vector>

// 定义节点的抽象类
class AbstractTeam
{
public:
	AbstractTeam(std::string name) : m_name(name) { }
	std::string getName() const
	{
		return m_name;
	}

	void setParent(AbstractTeam* team)
	{
		m_parent = team;
	}

	AbstractTeam* getParent() const
	{
		return m_parent;
	}

	virtual bool hasChild()
	{
		return false;
	}
	virtual void addChild(AbstractTeam* node) { }
	virtual void removeChild(AbstractTeam* node) { }
	virtual void fight() = 0;
	virtual void display() = 0;
	virtual ~AbstractTeam() { }

protected:
	std::string m_name;
	AbstractTeam* m_parent = nullptr;
};

// 叶子节点类
class LeafTeam : public AbstractTeam
{
public:
	using AbstractTeam::AbstractTeam;
	void fight() override
	{
		std::cout << m_parent->getName() + m_name + "与黑胡子的船员进行近距离的肉搏战......" << std::endl;
	}

	void display() override
	{
		std::cout << "我是" << m_parent->getName() << "下属的" << m_name << std::endl;
	}

	~LeafTeam()
	{
		std::cout << "我是" << m_parent->getName() << "下属的" << m_name
			<< ", 战斗已经结束, 拜拜......" << std::endl;
	}
};


// 管理者节点类
class ManagerTeam : public AbstractTeam
{
public:
	using AbstractTeam::AbstractTeam;
	bool hasChild()
	{
		return true;
	}

	void addChild(AbstractTeam* node) 
	{
		node->setParent(this);
		m_children.push_back(node);
	}

	void removeChild(AbstractTeam* node)
	{
		node->setParent(nullptr);
		m_children.remove(node);
	}

	void fight() override
	{
		std::cout << m_name + "和黑胡子的恶魔果实能力者站斗!!!" << std::endl;
	}

	void display() override
	{
		std::string info;
		for (const auto& item : m_children)
		{
			info += item->getName();
			if (item != m_children.back())
			{
				info += ", ";
			}
		}
		std::cout << m_name << "的船队是【" << info << "】" << std::endl;
	}

	const std::list<AbstractTeam*>& getChildren() const
	{
		return m_children;
	}

	~ManagerTeam()
	{
		std::cout << "我是" << m_name << ", 战斗已经结束, 拜拜......" << std::endl;
	}

private:
	std::list<AbstractTeam*> m_children;
};

#if 0

void gameover(AbstractTeam* root)
{
	if (root == nullptr)
	{
		return;
	}

	if (root->hasChild())
	{
		ManagerTeam* team = dynamic_cast<ManagerTeam*>(root);
		std::list<AbstractTeam*>& children = const_cast<std::list<AbstractTeam*>&>(team->getChildren());
		for (const auto& item : children)
		{
			gameover(item);
		}
	}

	delete root;
}

void fighting()
{
	AbstractTeam* root = new ManagerTeam("草帽海贼团");
	std::vector<std::string> teamNameList = {
		"俊美海贼团", "巴托俱乐部", "八宝水军", "艾迪欧海贼团",
		"咚塔塔海贼团", "巨兵海贼团", "约塔玛利亚大船团"
	};
	for (int i = 0; i < teamNameList.size(); ++i)
	{
		AbstractTeam* child = new ManagerTeam(teamNameList.at(i));
		root->addChild(child);
		if (i == teamNameList.size() - 1)
		{
			for (int j = 0; j < 9; ++j)
			{
				AbstractTeam* leaf = new LeafTeam("第" + std::to_string(j + 1) + "番队");
				child->addChild(leaf);
				leaf->display();
				leaf->fight();
			}
		}
		child->display();
		child->fight();
	}
	root->display();
	root->fight();

	std::cout << "------------------------------------------------" << std::endl;
	gameover(root);
}

int main()
{
	fighting();
	return 0;
}

#endif