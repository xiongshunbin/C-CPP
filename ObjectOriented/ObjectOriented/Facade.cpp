#include <iostream>

// 可乐系统
class CokeSystem
{
public:
	void imitCoke()
	{
		std::cout << "狮吼炮原料<可乐>已经注入完毕......" << std::endl;
	}
};

// 能量转换系统
class EnergySystem
{
public:
	void energyConvert()
	{
		std::cout << "已经将所有的可乐转换为了能量......" << std::endl;
	}
};

// 目标锁定系统
class AimLockSystem
{
public:
	void aimLock()
	{
		std::cout << "已经瞄准并且锁定了目标......" << std::endl;
	}
};

// 加农炮发射系统
class Cannon
{
public:
	void cannonFire()
	{
		std::cout << "狮吼炮正在向目标开火......" << std::endl;
	}
};

// 风来跑稳定系统
class WindCannon
{
public:
	void windCannonFire()
	{
		std::cout << "发射风来炮抵消后坐力稳定船身......" << std::endl;
	}
};

// 狮吼炮类
class LionCannon
{
public:
	LionCannon()
	{
		m_coke = new CokeSystem;
		m_energy = new EnergySystem;
		m_aimlock = new AimLockSystem;
		m_cannon = new Cannon;
		m_windCannon = new WindCannon;
	}
	~LionCannon()
	{
		delete m_coke;
		delete m_energy;
		delete m_aimlock;
		delete m_cannon;
		delete m_windCannon;
	}

	// 瞄准并锁定
	void aimAndLock()
	{
		m_coke->imitCoke();
		m_energy->energyConvert();
		m_aimlock->aimLock();
	}

	// 开火
	void fire()
	{
		m_cannon->cannonFire();
		m_windCannon->windCannonFire();
	}

private:
	CokeSystem* m_coke = nullptr;
	EnergySystem* m_energy = nullptr;
	AimLockSystem* m_aimlock = nullptr;
	Cannon* m_cannon = nullptr;
	WindCannon* m_windCannon = nullptr;
};

#if 0

int main()
{
	LionCannon* lionCannon = new LionCannon;
	lionCannon->aimAndLock();
	lionCannon->fire();
	delete lionCannon;
	return 0;
}

#endif