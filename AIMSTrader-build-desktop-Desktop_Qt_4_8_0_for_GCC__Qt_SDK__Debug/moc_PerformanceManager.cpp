/****************************************************************************
** Meta object code from reading C++ file 'PerformanceManager.h'
**
** Created: Tue May 8 23:26:10 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AIMSTrader/Platform/Performance/PerformanceManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PerformanceManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PerformanceManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      48,   20,   19,   19, 0x05,
     158,   97,   19,   19, 0x05,
     229,  225,   19,   19, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_PerformanceManager[] = {
    "PerformanceManager\0\0,profitableTrades,netProfit\0"
    "performanceUpdatedOnTrade(StrategyId,int,double)\0"
    "strategyId,profitableTrades,totalBought,totalSold,commission\0"
    "performanceUpdatedOnExecution(StrategyId,int,double,double,double)\0"
    ",,,\0tradesUpdated(StrategyId,int,int,int)\0"
};

void PerformanceManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PerformanceManager *_t = static_cast<PerformanceManager *>(_o);
        switch (_id) {
        case 0: _t->performanceUpdatedOnTrade((*reinterpret_cast< const StrategyId(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const double(*)>(_a[3]))); break;
        case 1: _t->performanceUpdatedOnExecution((*reinterpret_cast< const StrategyId(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const double(*)>(_a[3])),(*reinterpret_cast< const double(*)>(_a[4])),(*reinterpret_cast< const double(*)>(_a[5]))); break;
        case 2: _t->tradesUpdated((*reinterpret_cast< const StrategyId(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PerformanceManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PerformanceManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PerformanceManager,
      qt_meta_data_PerformanceManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PerformanceManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PerformanceManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PerformanceManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PerformanceManager))
        return static_cast<void*>(const_cast< PerformanceManager*>(this));
    return QObject::qt_metacast(_clname);
}

int PerformanceManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void PerformanceManager::performanceUpdatedOnTrade(const StrategyId _t1, const int _t2, const double _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PerformanceManager::performanceUpdatedOnExecution(const StrategyId _t1, const int _t2, const double _t3, const double _t4, const double _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PerformanceManager::tradesUpdated(const StrategyId _t1, const int _t2, const int _t3, const int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
