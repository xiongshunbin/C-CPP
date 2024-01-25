/****************************************************************************
** Meta object code from reading C++ file 'GamePanel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../GamePanel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GamePanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GamePanel_t {
    QByteArrayData data[16];
    char stringdata0[186];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GamePanel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GamePanel_t qt_meta_stringdata_GamePanel = {
    {
QT_MOC_LITERAL(0, 0, 9), // "GamePanel"
QT_MOC_LITERAL(1, 10, 14), // "onDispatchCard"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 21), // "onPlayerStatusChanged"
QT_MOC_LITERAL(4, 48, 7), // "Player*"
QT_MOC_LITERAL(5, 56, 6), // "player"
QT_MOC_LITERAL(6, 63, 25), // "GameControl::PlayerStatus"
QT_MOC_LITERAL(7, 89, 6), // "status"
QT_MOC_LITERAL(8, 96, 13), // "onGrabLordBet"
QT_MOC_LITERAL(9, 110, 3), // "bet"
QT_MOC_LITERAL(10, 114, 7), // "isFirst"
QT_MOC_LITERAL(11, 122, 14), // "onCardSelected"
QT_MOC_LITERAL(12, 137, 15), // "Qt::MouseButton"
QT_MOC_LITERAL(13, 153, 6), // "button"
QT_MOC_LITERAL(14, 160, 14), // "onUserPlayHand"
QT_MOC_LITERAL(15, 175, 10) // "onUserPass"

    },
    "GamePanel\0onDispatchCard\0\0"
    "onPlayerStatusChanged\0Player*\0player\0"
    "GameControl::PlayerStatus\0status\0"
    "onGrabLordBet\0bet\0isFirst\0onCardSelected\0"
    "Qt::MouseButton\0button\0onUserPlayHand\0"
    "onUserPass"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GamePanel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x08 /* Private */,
       3,    2,   45,    2, 0x08 /* Private */,
       8,    3,   50,    2, 0x08 /* Private */,
      11,    1,   57,    2, 0x08 /* Private */,
      14,    0,   60,    2, 0x08 /* Private */,
      15,    0,   61,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4, 0x80000000 | 6,    5,    7,
    QMetaType::Void, 0x80000000 | 4, QMetaType::Int, QMetaType::Bool,    5,    9,   10,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void GamePanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GamePanel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onDispatchCard(); break;
        case 1: _t->onPlayerStatusChanged((*reinterpret_cast< Player*(*)>(_a[1])),(*reinterpret_cast< GameControl::PlayerStatus(*)>(_a[2]))); break;
        case 2: _t->onGrabLordBet((*reinterpret_cast< Player*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 3: _t->onCardSelected((*reinterpret_cast< Qt::MouseButton(*)>(_a[1]))); break;
        case 4: _t->onUserPlayHand(); break;
        case 5: _t->onUserPass(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Player* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Player* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GamePanel::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_GamePanel.data,
    qt_meta_data_GamePanel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GamePanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GamePanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GamePanel.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int GamePanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
