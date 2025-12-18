#include <iostream>
#include "Observer.h"
#include "NewsAgency.h"

Observer::Observer(std::string name, NewsAgency* news) : m_name(name), m_news(news)
{
	m_news->attach(this);
}

void Observer::unsubscribe()
{
	m_news->detach(this);
}

#if 0

int main()
{
	// 发布者
	NewsAgency* news1 = new Morgans;
	NewsAgency* news2 = new Gossip;

	// 观察者(订阅者)
	Dragon* dragon = new Dragon("蒙奇·D·龙", news1);
	Shanks* shanks = new Shanks("香克斯", news1);
	Bartolomeo* barto = new Bartolomeo("巴托洛米奥", news2);

	// 由发布者推送消息
	news1->notify("蒙奇·D·路飞成为新世界的新的四皇之一, 赏金30亿贝里!!!");
	std::cout << "------------------------------------------------" << std::endl;
	news2->notify("女帝汉库克想要嫁给路飞, 给路飞生猴子, 哈哈哈...");

	delete news1;
	delete news2;
	delete dragon;
	delete shanks;
	delete barto;

	return 0;
}

#endif
