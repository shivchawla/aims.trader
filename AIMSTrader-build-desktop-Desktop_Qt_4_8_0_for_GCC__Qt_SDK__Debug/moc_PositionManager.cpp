/****************************************************************************
** Meta object code from reading C++ file 'PositionManager.h'
**
** Created: Tue May 8 23:26:11 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AIMSTrader/Platform/Position/PositionManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PositionManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PositionManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      66,   17,   16,   16, 0x05,
     157,  131,   16,   16, 0x05,
     214,  211,   16,   16, 0x05,
     264,  262,   16,   16, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_PositionManager[] = {
    "PositionManager\0\0"
    ",,quantity,avgFillPrice,positionValue,commission\0"
    "executionUpdated(StrategyId,PositionId,int,double,double,double)\0"
    ",,positionValue,netProfit\0"
    "lastPriceUpdated(StrategyId,PositionId,double,double)\0"
    ",,\0positionCreated(StrategyId,PositionId,TickerId)\0"
    ",\0positionRemoved(StrategyId,PositionId)\0"
};

void PositionManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PositionManager *_t = static_cast<PositionManager *>(_o);
        switch (_id) {
        case 0: _t->executionUpdated((*reinterpret_cast< const StrategyId(*)>(_a[1])),(*reinterpret_cast< const PositionId(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const double(*)>(_a[4])),(*reinterpret_cast< const double(*)>(_a[5])),(*reinterpret_cast< const double(*)>(_a[6]))); break;
        case 1: _t->lastPriceUpdated((*reinterpret_cast< const StrategyId(*)>(_a[1])),(*reinterpret_cast< const PositionId(*)>(_a[2])),(*reinterpret_cast< const double(*)>(_a[3])),(*reinterpret_cast< const double(*)>(_a[4]))); break;
        case 2: _t->positionCreated((*reinterpret_cast< const StrategyId(*)>(_a[1])),(*reinterpret_cast< const PositionId(*)>(_a[2])),(*reinterpret_cast< const TickerId(*)>(_a[3]))); break;
        case 3: _t->positionRemoved((*reinterpret_cast< const StrategyId(*)>(_a[1])),(*reinterpret_cast< const PositionId(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PositionManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PositionManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PositionManager,
      qt_meta_data_PositionManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PositionManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PositionManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PositionManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PositionManager))
        return static_cast<void*>(const_cast< PositionManager*>(this));
    return QObject::qt_metacast(_clname);
}

int PositionManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void PositionManager::executionUpdated(const StrategyId _t1, const PositionId _t2, const int _t3, const double _t4, const double _t5, const double _t6)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PositionManager::lastPriceUpdated(const StrategyId _t1, const PositionId _t2, const double _t3, const double _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PositionManager::positionCreated(const StrategyId _t1, const PositionId _t2, const TickerId _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void PositionManager::positionRemoved(const StrategyId _t1, const PositionId _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
