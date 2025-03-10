#include "IconHelper.h"
#include <QMutex>
#include <QFontDatabase>

IconHelper* IconHelper::self = nullptr;

IconHelper::IconHelper(QObject* parent) : QObject(parent)
{
	QFontDatabase fontDB;
	int fontID;

	// 添加 FontAwesome4
	if (!fontDB.families().contains(FONT_FAMILY_AWESOME4))
	{
		fontID = fontDB.addApplicationFont(FONT_PATH_AWESOME4);
		QStringList fontFamilies = fontDB.applicationFontFamilies(fontID);
		if (fontFamilies.count() > 0)
		{
			iconFontAwesome4 = QFont(FONT_FAMILY_AWESOME4);
			iconFontAwesome4.setHintingPreference(QFont::PreferNoHinting);
		}
	}

	// 添加 FontWeather
	if (!fontDB.families().contains(FONT_FAMILY_WEATHER))
	{
		fontID = fontDB.addApplicationFont(FONT_PATH_WEATHER);
		QStringList fontFamilies = fontDB.applicationFontFamilies(fontID);
		if (fontFamilies.count() > 0)
		{
			iconFontWeather = QFont(FONT_FAMILY_WEATHER);
			iconFontWeather.setHintingPreference(QFont::PreferNoHinting);
		}
	}

	// 添加 FontAlibaba
	if (!fontDB.families().contains(FONT_FAMILY_ALIBABA))
	{
		fontID = fontDB.addApplicationFont(FONT_PATH_ALIBABA);
		QStringList fontFamilies = fontDB.applicationFontFamilies(fontID);
		if (fontFamilies.count() > 0)
		{
			iconFontAlibaba = QFont(FONT_FAMILY_ALIBABA);
			iconFontAlibaba.setHintingPreference(QFont::PreferNoHinting);
		}
	}
}

/*
* 懒汉模式: 使用双重检查锁定，保证线程安全
*/
IconHelper* IconHelper::getInstance()
{
	// 第一次检查，避免不必要的锁
	if (self == nullptr)
	{
		QMutex mutex;
		QMutexLocker locker(&mutex);
		// 第二次检查，确保实例唯一
		if (self == nullptr)
			self = new IconHelper();
	}
	return self;
}

QList<int> IconHelper::getAwesome4Icons()
{
	int start = 0xf000;
	int end = 0xf2e0;

	// 0xf000~0xf2e0 范围内，有些是空的
	QList<int> emptyList = { 0xf00f, 0xf01f, 0xf020, 0xf03f, 0xf04f, 0xf05f, 0xf06f, 0xf07f, 0xf08f, 0xf09f, 0xf0af, 0xf0b3, 0xf0b4, 0xf0b5, 0xf0b6, 0xf0b7,
							0xf0b8, 0xf0b9, 0xf0ba, 0xf0bb, 0xf0bc, 0xf0bd, 0xf0be, 0xf0bf, 0xf0bf, 0xf0cf, 0xf0df, 0xf0ef, 0xf0ff, 0xf10f, 0xf116, 0xf117,
							0xf11f, 0xf12f, 0xf13f, 0xf14f, 0xf15f, 0xf16f, 0xf17f, 0xf18f, 0xf19f, 0xf1af, 0xf1bf, 0xf1cf, 0xf1df, 0xf1ef, 0xf1ff, 0xf20f,
							0xf21f, 0xf220, 0xf22e, 0xf22f, 0xf23f, 0xf24f, 0xf25f, 0xf26f, 0xf27f, 0xf28f, 0xf29f, 0xf2af, 0xf2bf, 0xf2cf, 0xf2df };

	QList<int> list;
	for (int icon = start; icon <= end; icon++) {
		// 0xf000~0xf2e0 范围内，有些是空的
		if (emptyList.contains(icon)) {
			continue;
		}
		list.append(icon);
	}

	return list;
}

QList<int> IconHelper::getWeatherIcons()
{
	QList<int> list;
	for (int i = 0xe900; i <= 0xe9cf; ++i)
		list.append(i);

	return list;
}

QList<int> IconHelper::getAlibabaIcons()
{
	QList<int> list;

	// 图表
	for (int i = 0xa000; i <= 0xa027; i++) {
		list.append(i);
	}

	// 仪表盘
	for (int i = 0xa100; i <= 0xa106; i++) {
		list.append(i);
	}

	// 表格
	for (int i = 0xa200; i <= 0xa20e; i++) {
		list.append(i);
	}

	// 日历
	for (int i = 0xa300; i <= 0xa30b; i++) {
		list.append(i);
	}

	//  树状图
	for (int i = 0xa400; i <= 0xa402; i++) {
		list.append(i);
	}

	// 标签页
	for (int i = 0xa500; i <= 0xa505; i++) {
		list.append(i);
	}

	// 最大化最小化
	for (int i = 0xa600; i <= 0xa603; i++) {
		list.append(i);
	}

	// 文本编辑器
	for (int i = 0xa700; i <= 0xa70a; i++) {
		list.append(i);
	}

	// 加号
	for (int i = 0xa900; i <= 0xa901; i++) {
		list.append(i);
	}

	// 天气
	for (int i = 0xaa00; i <= 0xaa0e; i++) {
		list.append(i);
	}

	// 换肤
	for (int i = 0xab00; i <= 0xab03; i++) {
		list.append(i);
	}

	return list;
}
