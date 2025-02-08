#ifndef _MANAGER_H_
#define _MANAGER_H_

// 可继承单例模板类
template<typename T>
class Manager
{
public:
	static T* instance()
	{
		if (!manager)
			manager = new T();

		return manager;
	}

private:
	static T* manager;

protected:
	Manager() = default;
	~Manager() = default;
	Manager(const Manager&) = delete;
	Manager& operator=(const Manager&) = delete;
};

template<typename T>
T* Manager<T>::manager = nullptr;

#endif	// !_MANAGER_H_