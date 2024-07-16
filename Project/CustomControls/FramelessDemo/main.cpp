#include "FramelessDemo.h"
#include <QtWidgets/QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QFont font;
	font.setFamily("Microsoft Yahei");
	font.setPixelSize(13);
	a.setFont(font);

    FramelessDemo w;

	// ´°¿Ú¾ÓÖÐÏÔÊ¾
	QDesktopWidget* desktop = QApplication::desktop();
	w.move((desktop->width() - w.width()) / 2, (desktop->height() - w.height()) / 2);

    w.show();
    return a.exec();
}
