/****************************************************************************
** Meta object code from reading C++ file 'editwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../editwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'editwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_EditWidget_t {
    QByteArrayData data[12];
    char stringdata0[162];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EditWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EditWidget_t qt_meta_stringdata_EditWidget = {
    {
QT_MOC_LITERAL(0, 0, 10), // "EditWidget"
QT_MOC_LITERAL(1, 11, 11), // "getVDataPtr"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 8), // "vdata_t*"
QT_MOC_LITERAL(4, 33, 5), // "vdata"
QT_MOC_LITERAL(5, 39, 18), // "xSliderValuechange"
QT_MOC_LITERAL(6, 58, 5), // "value"
QT_MOC_LITERAL(7, 64, 18), // "ySliderValuechange"
QT_MOC_LITERAL(8, 83, 18), // "zSliderValuechange"
QT_MOC_LITERAL(9, 102, 19), // "xSpinboxValuechange"
QT_MOC_LITERAL(10, 122, 19), // "ySpinboxValuechange"
QT_MOC_LITERAL(11, 142, 19) // "zSpinboxValuechange"

    },
    "EditWidget\0getVDataPtr\0\0vdata_t*\0vdata\0"
    "xSliderValuechange\0value\0ySliderValuechange\0"
    "zSliderValuechange\0xSpinboxValuechange\0"
    "ySpinboxValuechange\0zSpinboxValuechange"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EditWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x08 /* Private */,
       5,    1,   52,    2, 0x08 /* Private */,
       7,    1,   55,    2, 0x08 /* Private */,
       8,    1,   58,    2, 0x08 /* Private */,
       9,    1,   61,    2, 0x08 /* Private */,
      10,    1,   64,    2, 0x08 /* Private */,
      11,    1,   67,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,

       0        // eod
};

void EditWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        EditWidget *_t = static_cast<EditWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->getVDataPtr((*reinterpret_cast< vdata_t*(*)>(_a[1]))); break;
        case 1: _t->xSliderValuechange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->ySliderValuechange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->zSliderValuechange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->xSpinboxValuechange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->ySpinboxValuechange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->zSpinboxValuechange((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject EditWidget::staticMetaObject = {
    { &QOpenGLWidget::staticMetaObject, qt_meta_stringdata_EditWidget.data,
      qt_meta_data_EditWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *EditWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EditWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_EditWidget.stringdata0))
        return static_cast<void*>(const_cast< EditWidget*>(this));
    return QOpenGLWidget::qt_metacast(_clname);
}

int EditWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
