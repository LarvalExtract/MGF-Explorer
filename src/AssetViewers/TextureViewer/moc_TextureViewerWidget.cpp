/****************************************************************************
** Meta object code from reading C++ file 'TextureViewerWidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.0.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "TextureViewerWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TextureViewerWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.0.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TextureViewer__TextureViewerWidget_t {
    const uint offsetsAndSize[8];
    char stringdata0[75];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_TextureViewer__TextureViewerWidget_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_TextureViewer__TextureViewerWidget_t qt_meta_stringdata_TextureViewer__TextureViewerWidget = {
    {
QT_MOC_LITERAL(0, 34), // "TextureViewer::TextureViewerW..."
QT_MOC_LITERAL(35, 30), // "on_ToggleAlphaCheckBox_toggled"
QT_MOC_LITERAL(66, 0), // ""
QT_MOC_LITERAL(67, 7) // "checked"

    },
    "TextureViewer::TextureViewerWidget\0"
    "on_ToggleAlphaCheckBox_toggled\0\0checked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TextureViewer__TextureViewerWidget[] = {

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
       1,    1,   20,    2, 0x08,    0 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,

       0        // eod
};

void TextureViewer::TextureViewerWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TextureViewerWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_ToggleAlphaCheckBox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject TextureViewer::TextureViewerWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<IAssetViewerWidget::staticMetaObject>(),
    qt_meta_stringdata_TextureViewer__TextureViewerWidget.offsetsAndSize,
    qt_meta_data_TextureViewer__TextureViewerWidget,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_TextureViewer__TextureViewerWidget_t

, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>


>,
    nullptr
} };


const QMetaObject *TextureViewer::TextureViewerWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TextureViewer::TextureViewerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TextureViewer__TextureViewerWidget.stringdata0))
        return static_cast<void*>(this);
    return IAssetViewerWidget::qt_metacast(_clname);
}

int TextureViewer::TextureViewerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IAssetViewerWidget::qt_metacall(_c, _id, _a);
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
