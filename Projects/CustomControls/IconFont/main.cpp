#include "Widget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QFont font;
	font.setFamily("Microsoft Yahei");
	font.setPixelSize(14);
	a.setFont(font);
    Widget w;
    w.show();
    return a.exec();
}
