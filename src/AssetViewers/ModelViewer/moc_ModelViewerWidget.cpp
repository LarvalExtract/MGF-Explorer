/****************************************************************************
** Meta object code from reading C++ file 'ModelViewerWidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "ModelViewerWidget.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ModelViewerWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_ModelViewer__ModelViewerWidget_t {
    uint offsetsAndSizes[12];
    char stringdata0[31];
    char stringdata1[31];
    char stringdata2[1];
    char stringdata3[6];
    char stringdata4[31];
    char stringdata5[31];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ModelViewer__ModelViewerWidget_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_ModelViewer__ModelViewerWidget_t qt_meta_stringdata_ModelViewer__ModelViewerWidget = {
    {
        QT_MOC_LITERAL(0, 30),  // "ModelViewer::ModelViewerWidget"
        QT_MOC_LITERAL(31, 30),  // "on_lightPositionXInput_changed"
        QT_MOC_LITERAL(62, 0),  // ""
        QT_MOC_LITERAL(63, 5),  // "value"
        QT_MOC_LITERAL(69, 30),  // "on_lightPositionYInput_changed"
        QT_MOC_LITERAL(100, 30)   // "on_lightPositionZInput_changed"
    },
    "ModelViewer::ModelViewerWidget",
    "on_lightPositionXInput_changed",
    "",
    "value",
    "on_lightPositionYInput_changed",
    "on_lightPositionZInput_changed"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_ModelViewer__ModelViewerWidget[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   32,    2, 0x08,    1 /* Private */,
       4,    1,   35,    2, 0x08,    3 /* Private */,
       5,    1,   38,    2, 0x08,    5 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

Q_CONSTINIT const QMetaObject ModelViewer::ModelViewerWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<IAssetViewerWidget::staticMetaObject>(),
    qt_meta_stringdata_ModelViewer__ModelViewerWidget.offsetsAndSizes,
    qt_meta_data_ModelViewer__ModelViewerWidget,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_ModelViewer__ModelViewerWidget_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ModelViewerWidget, std::true_type>,
        // method 'on_lightPositionXInput_changed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_lightPositionYInput_changed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_lightPositionZInput_changed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void ModelViewer::ModelViewerWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ModelViewerWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_lightPositionXInput_changed((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->on_lightPositionYInput_changed((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->on_lightPositionZInput_changed((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *ModelViewer::ModelViewerWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ModelViewer::ModelViewerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ModelViewer__ModelViewerWidget.stringdata0))
        return static_cast<void*>(this);
    return IAssetViewerWidget::qt_metacast(_clname);
}

int ModelViewer::ModelViewerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IAssetViewerWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
