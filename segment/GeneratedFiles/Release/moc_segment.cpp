/****************************************************************************
** Meta object code from reading C++ file 'segment.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../segment.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'segment.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_segment_t {
    QByteArrayData data[23];
    char stringdata0[204];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_segment_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_segment_t qt_meta_stringdata_segment = {
    {
QT_MOC_LITERAL(0, 0, 7), // "segment"
QT_MOC_LITERAL(1, 8, 9), // "ShowAbout"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 8), // "ShowHelp"
QT_MOC_LITERAL(4, 28, 10), // "BrowseFile"
QT_MOC_LITERAL(5, 39, 10), // "BrowseDict"
QT_MOC_LITERAL(6, 50, 8), // "SaveFile"
QT_MOC_LITERAL(7, 59, 15), // "CheckDictEncode"
QT_MOC_LITERAL(8, 75, 15), // "CheckFileEncode"
QT_MOC_LITERAL(9, 91, 13), // "SetDictEncode"
QT_MOC_LITERAL(10, 105, 13), // "SetFileEncode"
QT_MOC_LITERAL(11, 119, 6), // "Search"
QT_MOC_LITERAL(12, 126, 3), // "Add"
QT_MOC_LITERAL(13, 130, 3), // "Del"
QT_MOC_LITERAL(14, 134, 11), // "DisplayDict"
QT_MOC_LITERAL(15, 146, 8), // "LoadDict"
QT_MOC_LITERAL(16, 155, 7), // "SetDict"
QT_MOC_LITERAL(17, 163, 3), // "Run"
QT_MOC_LITERAL(18, 167, 7), // "RunTest"
QT_MOC_LITERAL(19, 175, 9), // "ClearText"
QT_MOC_LITERAL(20, 185, 5), // "SetMM"
QT_MOC_LITERAL(21, 191, 5), // "SetMP"
QT_MOC_LITERAL(22, 197, 6) // "SetMax"

    },
    "segment\0ShowAbout\0\0ShowHelp\0BrowseFile\0"
    "BrowseDict\0SaveFile\0CheckDictEncode\0"
    "CheckFileEncode\0SetDictEncode\0"
    "SetFileEncode\0Search\0Add\0Del\0DisplayDict\0"
    "LoadDict\0SetDict\0Run\0RunTest\0ClearText\0"
    "SetMM\0SetMP\0SetMax"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_segment[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  119,    2, 0x0a /* Public */,
       3,    0,  120,    2, 0x0a /* Public */,
       4,    0,  121,    2, 0x0a /* Public */,
       5,    0,  122,    2, 0x0a /* Public */,
       6,    0,  123,    2, 0x0a /* Public */,
       7,    0,  124,    2, 0x0a /* Public */,
       8,    0,  125,    2, 0x0a /* Public */,
       9,    0,  126,    2, 0x0a /* Public */,
      10,    0,  127,    2, 0x0a /* Public */,
      11,    0,  128,    2, 0x0a /* Public */,
      12,    0,  129,    2, 0x0a /* Public */,
      13,    0,  130,    2, 0x0a /* Public */,
      14,    0,  131,    2, 0x0a /* Public */,
      15,    0,  132,    2, 0x0a /* Public */,
      16,    0,  133,    2, 0x0a /* Public */,
      17,    0,  134,    2, 0x0a /* Public */,
      18,    0,  135,    2, 0x0a /* Public */,
      19,    0,  136,    2, 0x0a /* Public */,
      20,    0,  137,    2, 0x0a /* Public */,
      21,    0,  138,    2, 0x0a /* Public */,
      22,    0,  139,    2, 0x0a /* Public */,

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

       0        // eod
};

void segment::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        segment *_t = static_cast<segment *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ShowAbout(); break;
        case 1: _t->ShowHelp(); break;
        case 2: _t->BrowseFile(); break;
        case 3: _t->BrowseDict(); break;
        case 4: _t->SaveFile(); break;
        case 5: _t->CheckDictEncode(); break;
        case 6: _t->CheckFileEncode(); break;
        case 7: _t->SetDictEncode(); break;
        case 8: _t->SetFileEncode(); break;
        case 9: _t->Search(); break;
        case 10: _t->Add(); break;
        case 11: _t->Del(); break;
        case 12: _t->DisplayDict(); break;
        case 13: _t->LoadDict(); break;
        case 14: _t->SetDict(); break;
        case 15: _t->Run(); break;
        case 16: _t->RunTest(); break;
        case 17: _t->ClearText(); break;
        case 18: _t->SetMM(); break;
        case 19: _t->SetMP(); break;
        case 20: _t->SetMax(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject segment::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_segment.data,
      qt_meta_data_segment,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *segment::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *segment::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_segment.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int segment::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 21;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
