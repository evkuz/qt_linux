/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[22];
    char stringdata0[551];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 16), // "Open_Port_Signal"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 8), // "portname"
QT_MOC_LITERAL(4, 38, 21), // "on_openButton_clicked"
QT_MOC_LITERAL(5, 60, 20), // "on_sitButton_clicked"
QT_MOC_LITERAL(6, 81, 25), // "on_stand_upButton_clicked"
QT_MOC_LITERAL(7, 107, 22), // "on_closeButton_clicked"
QT_MOC_LITERAL(8, 130, 35), // "on_servo_1_lineEdit_editingFi..."
QT_MOC_LITERAL(9, 166, 35), // "on_servo_2_lineEdit_editingFi..."
QT_MOC_LITERAL(10, 202, 35), // "on_servo_3_lineEdit_editingFi..."
QT_MOC_LITERAL(11, 238, 35), // "on_servo_4_lineEdit_editingFi..."
QT_MOC_LITERAL(12, 274, 35), // "on_servo_5_lineEdit_editingFi..."
QT_MOC_LITERAL(13, 310, 35), // "on_servo_6_lineEdit_editingFi..."
QT_MOC_LITERAL(14, 346, 24), // "on_set_posButton_clicked"
QT_MOC_LITERAL(15, 371, 28), // "on_S1_verSlider_valueChanged"
QT_MOC_LITERAL(16, 400, 5), // "value"
QT_MOC_LITERAL(17, 406, 28), // "on_S2_verSlider_valueChanged"
QT_MOC_LITERAL(18, 435, 28), // "on_S3_verSlider_valueChanged"
QT_MOC_LITERAL(19, 464, 28), // "on_S4_verSlider_valueChanged"
QT_MOC_LITERAL(20, 493, 28), // "on_S5_verSlider_valueChanged"
QT_MOC_LITERAL(21, 522, 28) // "on_S6_verSlider_valueChanged"

    },
    "MainWindow\0Open_Port_Signal\0\0portname\0"
    "on_openButton_clicked\0on_sitButton_clicked\0"
    "on_stand_upButton_clicked\0"
    "on_closeButton_clicked\0"
    "on_servo_1_lineEdit_editingFinished\0"
    "on_servo_2_lineEdit_editingFinished\0"
    "on_servo_3_lineEdit_editingFinished\0"
    "on_servo_4_lineEdit_editingFinished\0"
    "on_servo_5_lineEdit_editingFinished\0"
    "on_servo_6_lineEdit_editingFinished\0"
    "on_set_posButton_clicked\0"
    "on_S1_verSlider_valueChanged\0value\0"
    "on_S2_verSlider_valueChanged\0"
    "on_S3_verSlider_valueChanged\0"
    "on_S4_verSlider_valueChanged\0"
    "on_S5_verSlider_valueChanged\0"
    "on_S6_verSlider_valueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  104,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,  107,    2, 0x08 /* Private */,
       5,    0,  108,    2, 0x08 /* Private */,
       6,    0,  109,    2, 0x08 /* Private */,
       7,    0,  110,    2, 0x08 /* Private */,
       8,    0,  111,    2, 0x08 /* Private */,
       9,    0,  112,    2, 0x08 /* Private */,
      10,    0,  113,    2, 0x08 /* Private */,
      11,    0,  114,    2, 0x08 /* Private */,
      12,    0,  115,    2, 0x08 /* Private */,
      13,    0,  116,    2, 0x08 /* Private */,
      14,    0,  117,    2, 0x08 /* Private */,
      15,    1,  118,    2, 0x08 /* Private */,
      17,    1,  121,    2, 0x08 /* Private */,
      18,    1,  124,    2, 0x08 /* Private */,
      19,    1,  127,    2, 0x08 /* Private */,
      20,    1,  130,    2, 0x08 /* Private */,
      21,    1,  133,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, QMetaType::Int,   16,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Open_Port_Signal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->on_openButton_clicked(); break;
        case 2: _t->on_sitButton_clicked(); break;
        case 3: _t->on_stand_upButton_clicked(); break;
        case 4: _t->on_closeButton_clicked(); break;
        case 5: _t->on_servo_1_lineEdit_editingFinished(); break;
        case 6: _t->on_servo_2_lineEdit_editingFinished(); break;
        case 7: _t->on_servo_3_lineEdit_editingFinished(); break;
        case 8: _t->on_servo_4_lineEdit_editingFinished(); break;
        case 9: _t->on_servo_5_lineEdit_editingFinished(); break;
        case 10: _t->on_servo_6_lineEdit_editingFinished(); break;
        case 11: _t->on_set_posButton_clicked(); break;
        case 12: _t->on_S1_verSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->on_S2_verSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->on_S3_verSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->on_S4_verSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->on_S5_verSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->on_S6_verSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::Open_Port_Signal)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::Open_Port_Signal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
