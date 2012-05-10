/****************************************************************************
** Meta object code from reading C++ file 'InstrumentManager.h'
**
** Created: Wed May 9 00:34:55 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AIMSTrader/Platform/Trader/InstrumentManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'InstrumentManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_InstrumentManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   19,   18,   18, 0x05,
      62,   18,   18,   18, 0x05,
     106,   94,   18,   18, 0x05,
     136,   19,   18,   18, 0x05,
     175,   19,   18,   18, 0x05,
     210,   19,   18,   18, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_InstrumentManager[] = {
    "InstrumentManager\0\0,\0"
    "requestMarketDataToTA(TickerId,Contract)\0"
    "requestCancelDataToTA(TickerId)\0"
    "contractId,\0updateTickerId(long,TickerId)\0"
    "lastPriceUpdated(TickerId,TradeUpdate)\0"
    "quoteUpdated(TickerId,QuoteUpdate)\0"
    "instrumentAdded(TickerId,Contract)\0"
};

void InstrumentManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        InstrumentManager *_t = static_cast<InstrumentManager *>(_o);
        switch (_id) {
        case 0: _t->requestMarketDataToTA((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< const Contract(*)>(_a[2]))); break;
        case 1: _t->requestCancelDataToTA((*reinterpret_cast< const TickerId(*)>(_a[1]))); break;
        case 2: _t->updateTickerId((*reinterpret_cast< const long(*)>(_a[1])),(*reinterpret_cast< const TickerId(*)>(_a[2]))); break;
        case 3: _t->lastPriceUpdated((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< TradeUpdate(*)>(_a[2]))); break;
        case 4: _t->quoteUpdated((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< QuoteUpdate(*)>(_a[2]))); break;
        case 5: _t->instrumentAdded((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< const Contract(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData InstrumentManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject InstrumentManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_InstrumentManager,
      qt_meta_data_InstrumentManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &InstrumentManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *InstrumentManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *InstrumentManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_InstrumentManager))
        return static_cast<void*>(const_cast< InstrumentManager*>(this));
    return QObject::qt_metacast(_clname);
}

int InstrumentManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void InstrumentManager::requestMarketDataToTA(const TickerId _t1, const Contract & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void InstrumentManager::requestCancelDataToTA(const TickerId _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void InstrumentManager::updateTickerId(const long _t1, const TickerId _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void InstrumentManager::lastPriceUpdated(const TickerId _t1, TradeUpdate _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void InstrumentManager::quoteUpdated(const TickerId _t1, QuoteUpdate _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void InstrumentManager::instrumentAdded(const TickerId _t1, const Contract & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
