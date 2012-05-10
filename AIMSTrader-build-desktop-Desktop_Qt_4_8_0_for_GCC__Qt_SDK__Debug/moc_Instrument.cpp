/****************************************************************************
** Meta object code from reading C++ file 'Instrument.h'
**
** Created: Wed May 9 00:34:54 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AIMSTrader/Platform/Position/Instrument.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Instrument.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Instrument[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   12,   11,   11, 0x05,
      53,   12,   11,   11, 0x05,
     124,   88,   11,   11, 0x05,
     191,  171,   11,   11, 0x05,
     254,  230,   11,   11, 0x05,
     299,   12,   11,   11, 0x05,
     341,   12,   11,   11, 0x05,
     383,   12,   11,   11, 0x05,
     426,   12,   11,   11, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_Instrument[] = {
    "Instrument\0\0,\0lastPriceUpdated(TickerId,TradeUpdate)\0"
    "quoteUpdated(TickerId,QuoteUpdate)\0"
    "tickerId,field,price,canAutoExecute\0"
    "tickPriceUpdated(TickerId,TickType,double,int)\0"
    "tickerId,field,size\0"
    "tickSizeUpdated(TickerId,TickType,int)\0"
    "tickerId,tickType,value\0"
    "tickGenericUpdated(TickerId,TickType,double)\0"
    "oneMinuteSnapshotUpdated(TickerId,double)\0"
    "twoMinuteSnapshotUpdated(TickerId,double)\0"
    "fiveMinuteSnapshotUpdated(TickerId,double)\0"
    "tenMinuteSnapshotUpdated(TickerId,double)\0"
};

void Instrument::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Instrument *_t = static_cast<Instrument *>(_o);
        switch (_id) {
        case 0: _t->lastPriceUpdated((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< TradeUpdate(*)>(_a[2]))); break;
        case 1: _t->quoteUpdated((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< QuoteUpdate(*)>(_a[2]))); break;
        case 2: _t->tickPriceUpdated((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< const TickType(*)>(_a[2])),(*reinterpret_cast< const double(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4]))); break;
        case 3: _t->tickSizeUpdated((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< const TickType(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 4: _t->tickGenericUpdated((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< const TickType(*)>(_a[2])),(*reinterpret_cast< const double(*)>(_a[3]))); break;
        case 5: _t->oneMinuteSnapshotUpdated((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< const double(*)>(_a[2]))); break;
        case 6: _t->twoMinuteSnapshotUpdated((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< const double(*)>(_a[2]))); break;
        case 7: _t->fiveMinuteSnapshotUpdated((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< const double(*)>(_a[2]))); break;
        case 8: _t->tenMinuteSnapshotUpdated((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< const double(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Instrument::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Instrument::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Instrument,
      qt_meta_data_Instrument, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Instrument::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Instrument::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Instrument::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Instrument))
        return static_cast<void*>(const_cast< Instrument*>(this));
    return QObject::qt_metacast(_clname);
}

int Instrument::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void Instrument::lastPriceUpdated(const TickerId _t1, TradeUpdate _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Instrument::quoteUpdated(const TickerId _t1, QuoteUpdate _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Instrument::tickPriceUpdated(const TickerId _t1, const TickType _t2, const double _t3, const int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Instrument::tickSizeUpdated(const TickerId _t1, const TickType _t2, const int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Instrument::tickGenericUpdated(const TickerId _t1, const TickType _t2, const double _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Instrument::oneMinuteSnapshotUpdated(const TickerId _t1, const double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Instrument::twoMinuteSnapshotUpdated(const TickerId _t1, const double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Instrument::fiveMinuteSnapshotUpdated(const TickerId _t1, const double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void Instrument::tenMinuteSnapshotUpdated(const TickerId _t1, const double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_END_MOC_NAMESPACE
