#include <iostream>
#include <string>

/**
 * 适配器模式: 若适配器服务类(使用适配器类的类)没有派生类, 适配器类则可以使用多重继承的方式继承适配器服务类。
 * 作用: 为了解决两个对象、接口、模块之间的不兼容的问题。
 */

#define INHERIT

#ifndef INHERIT

class Foreigner
{
public:
	virtual std::string confession() = 0;
	void setResult(const std::string& res)
	{
		std::cout << "Panda say: " << res << std::endl;
	}
	virtual ~Foreigner() { }
};

class American : public Foreigner
{
public:
	std::string confession() override
	{
		return "American say: Sorry, I am guilty, and I have no shame!!!";
	}
};

class French : public Foreigner
{
public:
	std::string confession() override
	{
		return "French say: Sorry, I am a bandit, and I deserve to die!!!";
	}
};

class Panda
{
public:
	void recvMessage(const std::string& msg)
	{
		std::cout << msg << std::endl;
	}
	std::string sendMessage()
	{
		return "Bandits, murderers, and sinners cannot be pardoned or forgiven!";
	}
};

class AbstractChopper
{
public:
	AbstractChopper(Foreigner* foreigner) : m_foreigner(foreigner) { }
	virtual void translateToPanda() = 0;
	virtual void translateToHuman() = 0;
	virtual ~AbstractChopper() {}

protected:
	Panda m_panda;
	Foreigner* m_foreigner = nullptr;
};

class EnglishChopper : public AbstractChopper
{
public:
	using AbstractChopper::AbstractChopper;
	virtual void translateToPanda() override			// English to Animal
	{
		std::string msg = m_foreigner->confession();	
		// processing...
		m_panda.recvMessage(msg);
	}
	virtual void translateToHuman() override			// Animal to English
	{
		std::string msg = m_panda.sendMessage();
		// processing...
		m_foreigner->setResult(msg);
	}
};

class FrenchChopper : public AbstractChopper
{
public:
	using AbstractChopper::AbstractChopper;
	virtual void translateToPanda() override			// French to Animal
	{
		std::string msg = m_foreigner->confession();
		// processing...
		m_panda.recvMessage(msg);
	}
	virtual void translateToHuman() override			// Animal to French
	{
		std::string msg = m_panda.sendMessage();
		// processing...
		m_foreigner->setResult(msg);
	}
};

#else

class Foreigner
{
public:
	virtual std::string confession() = 0;
	void setResult(const std::string& res)
	{
		std::cout << "Panda say: " << res << std::endl;
	}
	virtual ~Foreigner() {}
};

class American : public Foreigner
{
public:
	std::string confession() override
	{
		return "American say: Sorry, I am guilty, and I have no shame!!!";
	}
};

class French : public Foreigner
{
public:
	std::string confession() override
	{
		return "French say: Sorry, I am a bandit, and I deserve to die!!!";
	}
};

class Panda
{
public:
	void recvMessage(const std::string& msg)
	{
		std::cout << msg << std::endl;
	}
	std::string sendMessage()
	{
		return "Bandits, murderers, and sinners cannot be pardoned or forgiven!";
	}
};

class AbstractChopper : protected Panda
{
public:
	AbstractChopper(Foreigner* foreigner) : m_foreigner(foreigner) { }
	virtual void translateToPanda() = 0;
	virtual void translateToHuman() = 0;
	virtual ~AbstractChopper() {}

protected:
	Foreigner* m_foreigner = nullptr;
};

class EnglishChopper : public AbstractChopper
{
public:
	using AbstractChopper::AbstractChopper;
	virtual void translateToPanda() override			// English to Animal
	{
		std::string msg = m_foreigner->confession();
		// processing...
		recvMessage(msg);
	}
	virtual void translateToHuman() override			// Animal to English
	{
		std::string msg = sendMessage();
		// processing...
		m_foreigner->setResult(msg);
	}
};

class FrenchChopper : public AbstractChopper
{
public:
	using AbstractChopper::AbstractChopper;
	virtual void translateToPanda() override			// French to Animal
	{
		std::string msg = m_foreigner->confession();
		// processing...
		recvMessage(msg);
	}
	virtual void translateToHuman() override			// Animal to French
	{
		std::string msg = sendMessage();
		// processing...
		m_foreigner->setResult(msg);
	}
};

#endif


#if 0

int main()
{
	Foreigner* foreigner = new American;
	AbstractChopper* adapter = new EnglishChopper(foreigner);
	adapter->translateToPanda();
	adapter->translateToHuman();

	delete foreigner;
	delete adapter;

	std::cout << "--------------------------------------------------------------------------" << std::endl;

	foreigner = new French;
	adapter = new FrenchChopper(foreigner);
	adapter->translateToPanda();
	adapter->translateToHuman();

	delete foreigner;
	delete adapter;

	return 0;
}

#endif
