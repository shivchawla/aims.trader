/****************************************************************************
** Meta object code from reading C++ file 'Strategy.h'
**
** Created: Tue May 8 23:26:08 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AIMSTrader/Platform/Strategy/Strategy.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Strategy.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Strategy[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,
      27,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      53,   43,    9,    9, 0x0a,
      89,   43,    9,    9, 0x0a,
     128,  125,    9,    9, 0x0a,
     176,  125,    9,    9, 0x0a,
     220,    9,    9,    9, 0x0a,
     236,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Strategy[] = {
    "Strategy\0\0startIndicator()\0stopIndicator()\0"
    "tickerId,\0onTradeUpdate(TickerId,TradeUpdate)\0"
    "onQuoteUpdate(TickerId,QuoteUpdate)\0"
    ",,\0onExecutionUpdate(OrderId,ExecutionStatus,bool)\0"
    "onTickPriceUpdate(TickerId,TickType,double)\0"
    "startStrategy()\0stopStrategy()\0"
};

void Strategy::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Strategy *_t = static_cast<Strategy *>(_o);
        switch (_id) {
        case 0: _t->startIndicator(); break;
        case 1: _t->stopIndicator(); break;
        case 2: _t->onTradeUpdate((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< const TradeUpdate(*)>(_a[2]))); break;
        case 3: _t->onQuoteUpdate((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< const QuoteUpdate(*)>(_a[2]))); break;
        case 4: _t->onExecutionUpdate((*reinterpret_cast< const OrderId(*)>(_a[1])),(*reinterpret_cast< const ExecutionStatus(*)>(_a[2])),(*reinterpret_cast< const bool(*)>(_a[3]))); break;
        case 5: _t->onTickPriceUpdate((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< const TickType(*)>(_a[2])),(*reinterpret_cast< const double(*)>(_a[3]))); break;
        case 6: _t->startStrategy(); break;
        case 7: _t->stopStrategy(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Strategy::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Strategy::staticMetaObject = {
    { &DataSubscriber::staticMetaObject, qt_meta_stringdata_Strategy,
      qt_meta_data_Strategy, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Strategy::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Strategy::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Strategy::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Strategy))
        return static_cast<void*>(const_cast< Strategy*>(this));
    return DataSubscriber::qt_metacast(_clname);
}

int Strategy::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DataSubscriber::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void Strategy::startIndicator()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Strategy::stopIndicator()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
