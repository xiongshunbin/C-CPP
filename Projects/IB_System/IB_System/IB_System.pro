QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bus.cpp \
    dialogAnimation/qtmaterialjbydialog.cpp \
    dialogAnimation/qtmaterialoverlaywidget.cpp \
    dialogAnimation/tidialog.cpp \
    dragwidget.cpp \
    framelesswidget.cpp \
    ib_changepassword.cpp \
    ib_includestation.cpp \
    ib_login.cpp \
    line.cpp \
    main.cpp \
    ib_mainwindow.cpp \
    mydialog.cpp \
    myitemdelegate.cpp \
    role.cpp \
    scheduling.cpp \
    sqldao.cpp \
    station.cpp \
    user.cpp

HEADERS += \
    bus.h \
    dialogAnimation/qtmaterialjbydialog.h \
    dialogAnimation/qtmaterialoverlaywidget.h \
    dialogAnimation/tidialog.h \
    dragwidget.h \
    framelesswidget.h \
    ib_changepassword.h \
    ib_includestation.h \
    ib_login.h \
    ib_mainwindow.h \
    line.h \
    mydialog.h \
    myitemdelegate.h \
    readonly.h \
    role.h \
    scheduling.h \
    sqldao.h \
    station.h \
    user.h

FORMS += \
    dialogAnimation/tidialog.ui \
    ib_changepassword.ui \
    ib_includestation.ui \
    ib_login.ui \
    ib_mainwindow.ui \
    mydialog.ui

RC_ICONS = title.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
