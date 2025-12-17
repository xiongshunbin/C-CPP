#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <vector>

// 数据类
class History
{
public:
	History(std::string msg) : m_msg(msg) { }
	std::string getHistory()
	{
		return m_msg;
	}
	~History()
	{
		std::cout << "数据被销毁" << std::endl;
	}

private:
	std::string m_msg;
};

// 事件的亲历者 - 脚盆鸡
class JiaoPenJi
{
public:
	// 脚盆鸡做的事情
	void setState(std::string msg)
	{
		m_msg = msg;
	}

	// 记录历史
	std::shared_ptr<History> saveHistory()
	{
		return std::make_shared<History>(m_msg);
	}

	// 得到历史信息 -> 不负责加载历史信息, 具体的加载动作是由记录者对象完成的
	void getStateFromHistory(std::shared_ptr<History> history)
	{
		m_msg = history->getHistory();
	}

	std::string getState()
	{
		return m_msg;
	}

	// 被打
	void beiDaddyDa()
	{
		std::cout << "脚盆鸡被兔子狠狠地揍了又揍, 终于承认了它们之前不承认的这些罪行: " << std::endl;
	}


private:
	std::string m_msg;
};

// 事件(数据)的记录者
class Recorder
{
public:
	void addHistory(int index, std::shared_ptr<History> history)
	{
		m_history.insert(std::make_pair(index, history));
	}

	std::shared_ptr<History> getHistory(int index)
	{
		if (m_history.find(index) != m_history.end())
		{
			return m_history[index];
		}
		return nullptr;
	}

private:
	std::map<int, std::shared_ptr<History>> m_history;
};

#if 0

int main()
{
	std::vector<std::string> msgs{
		"不承认偷了中国的中医!!!",
		"不承认发动了侵华战争!!!",
		"不承认南京大屠杀!!!",
		"不承认琉球群岛和钓鱼岛是中国的领土!!!",
		"不承认731部队的细菌和人体实验!!!",
		"不承认对我国妇女做出畜生行为!!!",
		"不承认从中国掠夺的数以亿计的资源和数以万计的文物!!!",
		"我干的龌龊事儿罄竹难书, 就是不承认......"
	};

	JiaoPenJi* jiaopen = new JiaoPenJi;
	Recorder* recorder = new Recorder;
	for (int i = 0; i < msgs.size(); ++i)
	{
		jiaopen->setState(msgs.at(i));
		recorder->addHistory(i, jiaopen->saveHistory());
	}

	jiaopen->beiDaddyDa();

	for (int i = 0; i < msgs.size(); ++i)
	{
		jiaopen->getStateFromHistory(recorder->getHistory(i));
		std::cout << "  --  " << jiaopen->getState() << std::endl;
	}

	delete jiaopen;
	delete recorder;

	return 0;
}

#endif
