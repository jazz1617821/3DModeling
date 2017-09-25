/****************************************************************************
** Meta object code from reading C++ file 'tdmmainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../tdmmainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tdmmainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TDMMainWindow_t {
    QByteArrayData data[11];
    char stringdata0[156];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TDMMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TDMMainWindow_t qt_meta_stringdata_TDMMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "TDMMainWindow"
QT_MOC_LITERAL(1, 14, 13), // "sentVModelPtr"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 9), // "vmodel_t*"
QT_MOC_LITERAL(4, 39, 12), // "sentVDataPtr"
QT_MOC_LITERAL(5, 52, 8), // "vdata_t*"
QT_MOC_LITERAL(6, 61, 23), // "on_actionSave_triggered"
QT_MOC_LITERAL(7, 85, 23), // "on_actionOpen_triggered"
QT_MOC_LITERAL(8, 109, 12), // "setAttribute"
QT_MOC_LITERAL(9, 122, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(10, 139, 16) // "QListWidgetItem*"

    },
    "TDMMainWindow\0sentVModelPtr\0\0vmodel_t*\0"
    "sentVDataPtr\0vdata_t*\0on_actionSave_triggered\0"
    "on_actionOpen_triggered\0setAttribute\0"
    "QTreeWidgetItem*\0QListWidgetItem*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TDMMainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    1,   47,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   50,    2, 0x08 /* Private */,
       7,    0,   51,    2, 0x08 /* Private */,
       8,    2,   52,    2, 0x08 /* Private */,
       8,    1,   57,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 5,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9, QMetaType::Int,    2,    2,
    QMetaType::Void, 0x80000000 | 10,    2,

       0        // eod
};

void TDMMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TDMMainWindow *_t = static_cast<TDMMainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sentVModelPtr((*reinterpret_cast< vmodel_t*(*)>(_a[1]))); break;
        case 1: _t->sentVDataPtr((*reinterpret_cast< vdata_t*(*)>(_a[1]))); break;
        case 2: _t->on_actionSave_triggered(); break;
        case 3: _t->on_actionOpen_triggered(); break;
        case 4: _t->setAttribute((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->setAttribute((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TDMMainWindow::*_t)(vmodel_t * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TDMMainWindow::sentVModelPtr)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TDMMainWindow::*_t)(vdata_t * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TDMMainWindow::sentVDataPtr)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject TDMMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_TDMMainWindow.data,
      qt_meta_data_TDMMainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TDMMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TDMMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TDMMainWindow.stringdata0))
        return static_cast<void*>(const_cast< TDMMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int TDMMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void TDMMainWindow::sentVModelPtr(vmodel_t * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TDMMainWindow::sentVDataPtr(vdata_t * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
