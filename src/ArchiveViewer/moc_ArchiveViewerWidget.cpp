/****************************************************************************
** Meta object code from reading C++ file 'ArchiveViewerWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "ArchiveViewerWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ArchiveViewerWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ArchiveViewer__ArchiveViewerWidget_t {
    QByteArrayData data[8];
    char stringdata0[130];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ArchiveViewer__ArchiveViewerWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ArchiveViewer__ArchiveViewerWidget_t qt_meta_stringdata_ArchiveViewer__ArchiveViewerWidget = {
    {
QT_MOC_LITERAL(0, 0, 34), // "ArchiveViewer::ArchiveViewerW..."
QT_MOC_LITERAL(1, 35, 28), // "on_treeView_selectionChanged"
QT_MOC_LITERAL(2, 64, 0), // ""
QT_MOC_LITERAL(3, 65, 11), // "QModelIndex"
QT_MOC_LITERAL(4, 77, 3), // "sel"
QT_MOC_LITERAL(5, 81, 5), // "desel"
QT_MOC_LITERAL(6, 87, 38), // "on_treeView_customContextMenu..."
QT_MOC_LITERAL(7, 126, 3) // "pos"

    },
    "ArchiveViewer::ArchiveViewerWidget\0"
    "on_treeView_selectionChanged\0\0QModelIndex\0"
    "sel\0desel\0on_treeView_customContextMenuRequested\0"
    "pos"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ArchiveViewer__ArchiveViewerWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x08 /* Private */,
       6,    1,   29,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    4,    5,
    QMetaType::Void, QMetaType::QPoint,    7,

       0        // eod
};

void ArchiveViewer::ArchiveViewerWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ArchiveViewerWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_treeView_selectionChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 1: _t->on_treeView_customContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ArchiveViewer::ArchiveViewerWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ArchiveViewer__ArchiveViewerWidget.data,
    qt_meta_data_ArchiveViewer__ArchiveViewerWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ArchiveViewer::ArchiveViewerWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ArchiveViewer::ArchiveViewerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ArchiveViewer__ArchiveViewerWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ArchiveViewer::ArchiveViewerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
