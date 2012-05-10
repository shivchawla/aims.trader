/****************************************************************************
** Meta object code from reading C++ file 'Indicator.h'
**
** Created: Tue May 8 23:26:09 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AIMSTrader/Platform/Indicator/Indicator.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Indicator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Indicator[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      32,   11,   10,   10, 0x0a,
      89,   68,   10,   10, 0x0a,
     128,  125,   10,   10, 0x0a,
     174,  172,   10,   10, 0x0a,
     215,   10,   10,   10, 0x0a,
     232,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Indicator[] = {
    "Indicator\0\0tickerId,tradeUpdate\0"
    "onTradeUpdate(TickerId,TradeUpdate)\0"
    "tickerId,quoteUpdate\0"
    "onQuoteUpdate(TickerId,QuoteUpdate)\0"
    ",,\0onTickPriceUpdate(TickerId,TickType,double)\0"
    ",\0updateOneMinuteSnapShot(TickerId,double)\0"
    "startIndicator()\0stopIndicator()\0"
};

void Indicator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Indicator *_t = static_cast<Indicator *>(_o);
        switch (_id) {
        case 0: _t->onTradeUpdate((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< const TradeUpdate(*)>(_a[2]))); break;
        case 1: _t->onQuoteUpdate((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< const QuoteUpdate(*)>(_a[2]))); break;
        case 2: _t->onTickPriceUpdate((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< const TickType(*)>(_a[2])),(*reinterpret_cast< const double(*)>(_a[3]))); break;
        case 3: _t->updateOneMinuteSnapShot((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< const double(*)>(_a[2]))); break;
        case 4: _t->startIndicator(); break;
        case 5: _t->stopIndicator(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Indicator::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Indicator::staticMetaObject = {
    { &DataSubscriber::staticMetaObject, qt_meta_stringdata_Indicator,
      qt_meta_data_Indicator, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Indicator::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Indicator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Indicator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Indicator))
        return static_cast<void*>(const_cast< Indicator*>(this));
    return DataSubscriber::qt_metacast(_clname);
}

int Indicator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DataSubscriber::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
