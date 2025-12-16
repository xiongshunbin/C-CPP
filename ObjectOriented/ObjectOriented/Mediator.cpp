#include "Mediator.h"
#include "Country.h"

void MediatorOrg::addMember(Country* country)
{
	if (country != nullptr)
	{
		country->setMediator(this);
		m_countryMap.insert(std::make_pair(country->getName(), country));
	}
}

void WorldGovt::declare(std::string msg, Country* country, std::string name)
{
	if (m_countryMap.find(name) != m_countryMap.end())
	{
		std::string  str = msg + "【来自: " + country->getName() + "】";
		m_countryMap[name]->setMessage(str);
	}
}

void GeMingArmy::declare(std::string msg, Country* country, std::string name)
{
	std::string  str = msg + "【来自: " + country->getName() + "】";
	for (const auto& item : m_countryMap)
	{
		if (item.second == country)
		{
			continue;
		}
		item.second->setMessage(str);
	}
}

#if 0

int main()
{
	// 世界政府
	WorldGovt* world = new WorldGovt;
	// 加盟国对象
	Alabasta* alaba = new Alabasta;
	Dressrosa* dress = new Dressrosa;
	world->addMember(alaba);
	world->addMember(dress);

	alaba->declare("德雷斯罗萨倒卖军火, 搞得我国连年打仗, 必须给个说法!!!", dress->getName());
	dress->declare("天龙人都和我多弗朗明哥做生意, 你算老几, 呸!!!", alaba->getName());

	// 革命军
	GeMingArmy* geming = new GeMingArmy;
	// 加盟国对象
	Lulusia* lulu = new Lulusia;
	Kamabaka* kama = new Kamabaka;
	geming->addMember(lulu);
	geming->addMember(kama);

	lulu->declare("我草, 我的国家被伊姆毁灭了!!!");

	delete world;
	delete geming;
	delete alaba;
	delete dress;
	delete lulu;
	delete kama;

	return 0;
}

#endif
