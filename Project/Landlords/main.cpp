#include "GamePanel.h"
#include <QtWidgets/QApplication>
#include <QResource>
#include "Loading.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<Cards>("Cards&");
    qRegisterMetaType<Cards>("Cards");
    QResource::registerResource("./resource.rcc");
    Loading w;
    w.show();
    return a.exec();
}
