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
    QByteArrayData data[9];
    char stringdata0[102];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EditWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EditWidget_t qt_meta_stringdata_EditWidget = {
    {
QT_MOC_LITERAL(0, 0, 10), // "EditWidget"
QT_MOC_LITERAL(1, 11, 18), // "xSliderValuechange"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 5), // "value"
QT_MOC_LITERAL(4, 37, 18), // "ySliderValuechange"
QT_MOC_LITERAL(5, 56, 18), // "zSliderValuechange"
QT_MOC_LITERAL(6, 75, 11), // "getVDataPtr"
QT_MOC_LITERAL(7, 87, 8), // "vdata_t*"
QT_MOC_LITERAL(8, 96, 5) // "vdata"

    },
    "EditWidget\0xSliderValuechange\0\0value\0"
    "ySliderValuechange\0zSliderValuechange\0"
    "getVDataPtr\0vdata_t*\0vdata"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EditWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       4,    1,   37,    2, 0x06 /* Public */,
       5,    1,   40,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   43,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void EditWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        EditWidget *_t = static_cast<EditWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->xSliderValuechange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->ySliderValuechange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->zSliderValuechange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->getVDataPtr((*reinterpret_cast< vdata_t*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (EditWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EditWidget::xSliderValuechange)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (EditWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EditWidget::ySliderValuechange)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (EditWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EditWidget::zSliderValuechange)) {
                *result = 2;
                return;
            }
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
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void EditWidget::xSliderValuechange(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void EditWidget::ySliderValuechange(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void EditWidget::zSliderValuechange(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
