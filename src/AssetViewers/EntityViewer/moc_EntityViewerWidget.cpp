/****************************************************************************
** Meta object code from reading C++ file 'EntityViewerWidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.0.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "EntityViewerWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EntityViewerWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.0.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_EntityViewer__EntityViewerWidget_t {
    const uint offsetsAndSize[12];
    char stringdata0[91];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_EntityViewer__EntityViewerWidget_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_EntityViewer__EntityViewerWidget_t qt_meta_stringdata_EntityViewer__EntityViewerWidget = {
    {
QT_MOC_LITERAL(0, 32), // "EntityViewer::EntityViewerWidget"
QT_MOC_LITERAL(33, 34), // "on_EntityTreeView_selectionCh..."
QT_MOC_LITERAL(68, 0), // ""
QT_MOC_LITERAL(69, 11), // "QModelIndex"
QT_MOC_LITERAL(81, 3), // "sel"
QT_MOC_LITERAL(85, 5) // "desel"

    },
    "EntityViewer::EntityViewerWidget\0"
    "on_EntityTreeView_selectionChanged\0\0"
    "QModelIndex\0sel\0desel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EntityViewer__EntityViewerWidget[] = {

 // content:
       9,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   20,    2, 0x08,    0 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    4,    5,

       0        // eod
};

void EntityViewer::EntityViewerWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<EntityViewerWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_EntityTreeView_selectionChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject EntityViewer::EntityViewerWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_EntityViewer__EntityViewerWidget.offsetsAndSize,
    qt_meta_data_EntityViewer__EntityViewerWidget,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_EntityViewer__EntityViewerWidget_t

, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>, QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>


>,
    nullptr
} };


const QMetaObject *EntityViewer::EntityViewerWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EntityViewer::EntityViewerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_EntityViewer__EntityViewerWidget.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "IAssetViewerWidget"))
        return static_cast< IAssetViewerWidget*>(this);
    return QWidget::qt_metacast(_clname);
}

int EntityViewer::EntityViewerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
