#include "ib_mainwindow.h"

#include <QApplication>
#include "sqldao.h"

int main(int argc, char *argv[])
{
    // 消除笔记本分辨率对窗体实际大小的影响
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Floor);

    QApplication a(argc, argv);
    ib_mainWindow w;

    // 先显示登录窗口，验证用户信息
    // w.show();

    return a.exec();
}
