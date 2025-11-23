#include <iostream>
#include <memory>
#include <mutex>

/**
 * CRTP(Curiously Recurring Template Pattern)单例模式实现
 */

template<typename T>
class SingletonBase
{
protected:
	SingletonBase() = default;
	SingletonBase(const SingletonBase&) = delete;
	SingletonBase& operator=(const SingletonBase&) = delete;

public:
	static std::shared_ptr<T> getInstance()
	{
		static std::once_flag flag;
		std::call_once(flag, []() {
			m_instance = std::shared_ptr<T>(new T);
		});
		return m_instance;
	}
	virtual ~SingletonBase() = default;

protected:
	static std::shared_ptr<T> m_instance;
};

template<typename T>
std::shared_ptr<T> SingletonBase<T>::m_instance = nullptr;

class Singleton : public SingletonBase<Singleton>
{
	friend class SingletonBase<Singleton>;

private:
	Singleton() = default;

public:
	void showMessage()
	{
		std::cout << "I am a Singleton!" << std::endl;
	}

	~Singleton()
	{
		std::cout << "Singleton is distructed!" << std::endl;
	}
};

#if 1

int main()
{
	Singleton::getInstance()->showMessage();
	return 0;
}

#endif
