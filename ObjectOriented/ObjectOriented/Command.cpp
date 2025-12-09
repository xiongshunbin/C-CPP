#include <iostream>
#include <string>
#include <map>
#include <list>
#include <algorithm>

// 厨师 -> 命令的执行者, 若未收到命令则不进行工作
class CookerZeff
{
public:
	void makeDSX()
	{
		std::cout << "开始烹饪地三鲜...";
	}

	void makeYXRS()
	{
		std::cout << "开始烹饪鱼香肉丝...";
	}

	void makeGBJD()
	{
		std::cout << "开始烹饪宫保鸡丁...";
	}

	void makeHSPG()
	{
		std::cout << "开始烹饪红烧排骨...";
	}
};

// 命令
class AbstractCommand
{
public:
	AbstractCommand(CookerZeff* zeff) : m_cooker(zeff) { }
	virtual void execute() = 0;
	virtual std::string name() = 0;
	bool getIsProcessed()
	{
		return m_isProcessed;
	}

	virtual ~AbstractCommand() { }

protected:
	CookerZeff* m_cooker = nullptr;
	bool m_isProcessed = false;
};

class DSXCommand : public AbstractCommand
{
public:
	using AbstractCommand::AbstractCommand;
	virtual void execute() override
	{
		m_cooker->makeDSX();
		m_isProcessed = true;
	}

	virtual std::string name() override
	{
		return "地三鲜";
	}
};

class YXRSCommand : public AbstractCommand
{
public:
	using AbstractCommand::AbstractCommand;
	virtual void execute() override
	{
		m_cooker->makeYXRS();
		m_isProcessed = true;
	}

	virtual std::string name() override
	{
		return "鱼香肉丝";
	}
};

class GBJDCommand : public AbstractCommand
{
public:
	using AbstractCommand::AbstractCommand;
	virtual void execute() override
	{
		m_cooker->makeGBJD();
		m_isProcessed = true;
	}

	virtual std::string name() override
	{
		return "宫保鸡丁";
	}
};

class HSPGCommand : public AbstractCommand
{
public:
	using AbstractCommand::AbstractCommand;
	virtual void execute() override
	{
		m_cooker->makeHSPG();
		m_isProcessed = true;
	}

	virtual std::string name() override
	{
		return "红烧排骨";
	}
};

// 服务员 - 命令的调用者
class WaiterLuffy
{
public:
	// 点餐
	void setOrder(int index, AbstractCommand* command)
	{
		std::cout << index << "号桌点了" << command->name() << std::endl;
		if (command->name() == "鱼香肉丝")
		{
			std::cout << "没有鱼了, 做不了鱼香肉丝, 点个别的菜吧..." << std::endl;
			return;
		}

		if (m_cmdList.find(index) == m_cmdList.end())
		{
			std::list<AbstractCommand*> myList{ command };
			m_cmdList.insert(std::make_pair(index, myList));
		}
		else
		{
			m_cmdList[index].push_back(command);
		}
	}

	// 撤销订单
	void cancelOrder(int index, AbstractCommand* command)
	{
		auto indexOfCmds = m_cmdList.find(index);
		if (indexOfCmds != m_cmdList.end())
		{
			std::list<AbstractCommand*>& cmdList = indexOfCmds->second;
			auto cmd = std::find(cmdList.begin(), cmdList.end(), command);
			if (cmd != cmdList.end() && !(*cmd)->getIsProcessed())
			{
				cmdList.remove(*cmd);
				std::cout << index << "号桌, 撤销了" << command->name() << std::endl;
			}
		}
	}

	// 结账
	void checkOut(int index)
	{
		if (m_cmdList.find(index) != m_cmdList.end())
		{
			std::cout << "第[" << index << "]号桌的顾客点的菜是:【";
			for (const auto& item : m_cmdList[index])
			{
				std::cout << item->name();
				if (item != m_cmdList[index].back())
				{
					std::cout << ", ";
				}
			}
			std::cout << "】" << std::endl;
			m_cmdList.erase(index);
		}
	}

	// 下单 -> 调用命令, 要求厨师执行某些命令
	void notify(int index)
	{
		for (const auto& item : m_cmdList[index])
		{
			item->execute();
			std::cout << index << "号桌" << std::endl;
		}
	}

private:
	std::map<int, std::list<AbstractCommand*>> m_cmdList;
};

#if 0

int main()
{
	CookerZeff* zeff = new CookerZeff;
	WaiterLuffy* luffy = new WaiterLuffy;

	DSXCommand* dsx = new DSXCommand(zeff);
	YXRSCommand* yxrs = new YXRSCommand(zeff);
	GBJDCommand* gbjd = new GBJDCommand(zeff);
	HSPGCommand* hspg = new HSPGCommand(zeff);

	std::cout << "-----------------顾客开始点餐-----------------" << std::endl;
	luffy->setOrder(1, dsx);
	luffy->setOrder(1, yxrs);
	luffy->setOrder(1, gbjd);
	luffy->setOrder(1, hspg);

	luffy->setOrder(2, dsx);
	luffy->setOrder(2, hspg);
	luffy->setOrder(2, gbjd);

	std::cout << "-------------------撤销订单-------------------" << std::endl;
	luffy->cancelOrder(1, dsx);

	std::cout << "-------------------开始下单-------------------" << std::endl;
	luffy->notify(1);
	luffy->notify(2);

	std::cout << "---------------------结账---------------------" << std::endl;
	luffy->checkOut(1);
	luffy->checkOut(2);

	delete zeff;
	delete luffy;
	delete dsx;
	delete yxrs;
	delete gbjd;
	delete hspg;

	return 0;
}

#endif