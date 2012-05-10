/****************************************************************************
** Meta object code from reading C++ file 'OpenOrder.h'
**
** Created: Tue May 8 23:26:12 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AIMSTrader/Platform/Position/OpenOrder.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'OpenOrder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_OpenOrder[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   11,   10,   10, 0x05,
      59,   57,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      98,   10,   10,   10, 0x0a,
     121,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_OpenOrder[] = {
    "OpenOrder\0\0,,\0orderUpdated(OrderId,ExecutionStatus,bool)\0"
    ",\0statusUpdated(OrderId,ExecutionStatus)\0"
    "updateOrder(Execution)\0reset()\0"
};

void OpenOrder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        OpenOrder *_t = static_cast<OpenOrder *>(_o);
        switch (_id) {
        case 0: _t->orderUpdated((*reinterpret_cast< const OrderId(*)>(_a[1])),(*reinterpret_cast< const ExecutionStatus(*)>(_a[2])),(*reinterpret_cast< const bool(*)>(_a[3]))); break;
        case 1: _t->statusUpdated((*reinterpret_cast< const OrderId(*)>(_a[1])),(*reinterpret_cast< const ExecutionStatus(*)>(_a[2]))); break;
        case 2: _t->updateOrder((*reinterpret_cast< const Execution(*)>(_a[1]))); break;
        case 3: _t->reset(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData OpenOrder::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OpenOrder::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_OpenOrder,
      qt_meta_data_OpenOrder, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OpenOrder::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OpenOrder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OpenOrder::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OpenOrder))
        return static_cast<void*>(const_cast< OpenOrder*>(this));
    return QObject::qt_metacast(_clname);
}

int OpenOrder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void OpenOrder::orderUpdated(const OrderId _t1, const ExecutionStatus & _t2, const bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void OpenOrder::statusUpdated(const OrderId _t1, const ExecutionStatus & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
