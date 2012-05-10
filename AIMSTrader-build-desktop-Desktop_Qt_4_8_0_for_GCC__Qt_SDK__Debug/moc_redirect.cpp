/****************************************************************************
** Meta object code from reading C++ file 'redirect.h'
**
** Created: Tue May 8 23:26:22 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AIMSTrader/Platform/Utils/redirect.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'redirect.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CRedirect[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,
      37,   28,   10,   10, 0x05,
      65,   28,   10,   10, 0x05,
      93,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     112,   10,   10,   10, 0x08,
     138,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CRedirect[] = {
    "CRedirect\0\0OnChildStarted()\0szOutput\0"
    "OnChildStdOutWrite(QString)\0"
    "OnChildStdErrWrite(QString)\0"
    "OnChildTerminate()\0readyReadStandardOutput()\0"
    "readyReadStandardError()\0"
};

void CRedirect::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CRedirect *_t = static_cast<CRedirect *>(_o);
        switch (_id) {
        case 0: _t->OnChildStarted(); break;
        case 1: _t->OnChildStdOutWrite((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->OnChildStdErrWrite((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->OnChildTerminate(); break;
        case 4: _t->readyReadStandardOutput(); break;
        case 5: _t->readyReadStandardError(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CRedirect::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CRedirect::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CRedirect,
      qt_meta_data_CRedirect, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CRedirect::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CRedirect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CRedirect::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CRedirect))
        return static_cast<void*>(const_cast< CRedirect*>(this));
    return QObject::qt_metacast(_clname);
}

int CRedirect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void CRedirect::OnChildStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CRedirect::OnChildStdOutWrite(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CRedirect::OnChildStdErrWrite(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CRedirect::OnChildTerminate()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
