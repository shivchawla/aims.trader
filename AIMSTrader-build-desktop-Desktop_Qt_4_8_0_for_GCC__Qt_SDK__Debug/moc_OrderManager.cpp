/****************************************************************************
** Meta object code from reading C++ file 'OrderManager.h'
**
** Created: Tue May 8 23:26:15 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AIMSTrader/Platform/Trader/OrderManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'OrderManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_OrderManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   14,   13,   13, 0x05,
      72,   63,   13,   13, 0x05,
     102,   98,   13,   13, 0x05,
     145,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     167,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_OrderManager[] = {
    "OrderManager\0\0,,\0"
    "requestPlaceOrdertoTA(OrderId,Order,Contract)\0"
    "contract\0requestTickerId(Contract)\0"
    ",,,\0orderPlaced(OrderId,Order,Contract,String)\0"
    "orderDeleted(OrderId)\0printThreadId()\0"
};

void OrderManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        OrderManager *_t = static_cast<OrderManager *>(_o);
        switch (_id) {
        case 0: _t->requestPlaceOrdertoTA((*reinterpret_cast< const OrderId(*)>(_a[1])),(*reinterpret_cast< const Order(*)>(_a[2])),(*reinterpret_cast< const Contract(*)>(_a[3]))); break;
        case 1: _t->requestTickerId((*reinterpret_cast< const Contract(*)>(_a[1]))); break;
        case 2: _t->orderPlaced((*reinterpret_cast< const OrderId(*)>(_a[1])),(*reinterpret_cast< const Order(*)>(_a[2])),(*reinterpret_cast< const Contract(*)>(_a[3])),(*reinterpret_cast< const String(*)>(_a[4]))); break;
        case 3: _t->orderDeleted((*reinterpret_cast< const OrderId(*)>(_a[1]))); break;
        case 4: _t->printThreadId(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData OrderManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OrderManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_OrderManager,
      qt_meta_data_OrderManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OrderManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OrderManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OrderManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OrderManager))
        return static_cast<void*>(const_cast< OrderManager*>(this));
    return QObject::qt_metacast(_clname);
}

int OrderManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void OrderManager::requestPlaceOrdertoTA(const OrderId _t1, const Order & _t2, const Contract & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void OrderManager::requestTickerId(const Contract & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void OrderManager::orderPlaced(const OrderId _t1, const Order & _t2, const Contract & _t3, const String & _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void OrderManager::orderDeleted(const OrderId _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
