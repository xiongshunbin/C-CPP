#pragma once
#include <QObject>
#include <QFont>

// FontAwesome4
#define FONT_PATH_AWESOME4		":/font/fontawesome-webfont.ttf"
#define FONT_FAMILY_AWESOME4	"FontAwesome"

// FontWeather
#define FONT_PATH_WEATHER		":/font/pe-icon-set-weather.ttf"
#define FONT_FAMILY_WEATHER		"FontWeather"

// FontAlibaba
#define FONT_PATH_ALIBABA		":/font/iconfont.ttf"
#define FONT_FAMILY_ALIBABA		"FontAlibaba"


class IconHelper : public QObject
{
	Q_OBJECT
private:
	static IconHelper* self;
	explicit IconHelper(QObject* parent = nullptr);
	~IconHelper() = default;
	IconHelper(const IconHelper&) = delete;
	IconHelper& operator=(const IconHelper&) = delete;

public:
	static IconHelper* getInstance();
	QList<int> getAwesome4Icons();
	QList<int> getWeatherIcons();
	QList<int> getAlibabaIcons();

private:
	QFont iconFontAwesome4;
	QFont iconFontWeather;
	QFont iconFontAlibaba;
};