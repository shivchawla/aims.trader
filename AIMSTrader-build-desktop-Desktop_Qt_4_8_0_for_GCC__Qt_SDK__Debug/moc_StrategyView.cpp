/****************************************************************************
** Meta object code from reading C++ file 'StrategyView.h'
**
** Created: Tue May 8 23:26:18 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AIMSTrader/Platform/View/StrategyView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'StrategyView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_StrategyView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      23,   13,   13,   13, 0x0a,
      32,   13,   13,   13, 0x0a,
     105,   54,   13,   13, 0x0a,
     188,  160,   13,   13, 0x0a,
     239,  225,   13,   13, 0x0a,
     311,  270,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_StrategyView[] = {
    "StrategyView\0\0closed()\0update()\0"
    "onPerformanceUpdate()\0"
    ",profitableTrades,totalBought,totalSold,commission\0"
    "onExecutionUpdate(StrategyId,int,double,double,double)\0"
    ",profitableTrades,netProfit\0"
    "onTradeUpdate(StrategyId,int,double)\0"
    ",strategyName\0addStrategy(StrategyId,String)\0"
    "strategyId,trades,longTrades,shortTrades\0"
    "updateTrades(StrategyId,int,int,int)\0"
};

void StrategyView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        StrategyView *_t = static_cast<StrategyView *>(_o);
        switch (_id) {
        case 0: _t->closed(); break;
        case 1: _t->update(); break;
        case 2: _t->onPerformanceUpdate(); break;
        case 3: _t->onExecutionUpdate((*reinterpret_cast< const StrategyId(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const double(*)>(_a[3])),(*reinterpret_cast< const double(*)>(_a[4])),(*reinterpret_cast< const double(*)>(_a[5]))); break;
        case 4: _t->onTradeUpdate((*reinterpret_cast< const StrategyId(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const double(*)>(_a[3]))); break;
        case 5: _t->addStrategy((*reinterpret_cast< const StrategyId(*)>(_a[1])),(*reinterpret_cast< const String(*)>(_a[2]))); break;
        case 6: _t->updateTrades((*reinterpret_cast< const StrategyId(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData StrategyView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject StrategyView::staticMetaObject = {
    { &TableView<StrategyView>::staticMetaObject, qt_meta_stringdata_StrategyView,
      qt_meta_data_StrategyView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &StrategyView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *StrategyView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *StrategyView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StrategyView))
        return static_cast<void*>(const_cast< StrategyView*>(this));
    return TableView<StrategyView>::qt_metacast(_clname);
}

int StrategyView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TableView<StrategyView>::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void StrategyView::closed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
