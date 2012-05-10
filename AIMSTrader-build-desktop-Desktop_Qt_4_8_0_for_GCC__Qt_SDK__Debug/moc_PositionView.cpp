/****************************************************************************
** Meta object code from reading C++ file 'PositionView.h'
**
** Created: Tue May 8 23:26:28 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AIMSTrader/Platform/View/PositionView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PositionView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PositionView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      23,   13,   13,   13, 0x0a,
      35,   32,   13,   13, 0x0a,
     128,   79,   13,   13, 0x0a,
     220,  194,   13,   13, 0x0a,
     273,  271,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PositionView[] = {
    "PositionView\0\0closed()\0update()\0,,\0"
    "addPosition(StrategyId,PositionId,TickerId)\0"
    ",,quantity,avgFillPrice,positionValue,commission\0"
    "onExecutionUpdate(StrategyId,PositionId,int,double,double,double)\0"
    ",,positionValue,netProfit\0"
    "onTradeUpdate(StrategyId,PositionId,double,double)\0"
    ",\0removePosition(StrategyId,PositionId)\0"
};

void PositionView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PositionView *_t = static_cast<PositionView *>(_o);
        switch (_id) {
        case 0: _t->closed(); break;
        case 1: _t->update(); break;
        case 2: _t->addPosition((*reinterpret_cast< const StrategyId(*)>(_a[1])),(*reinterpret_cast< const PositionId(*)>(_a[2])),(*reinterpret_cast< const TickerId(*)>(_a[3]))); break;
        case 3: _t->onExecutionUpdate((*reinterpret_cast< const StrategyId(*)>(_a[1])),(*reinterpret_cast< const PositionId(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const double(*)>(_a[4])),(*reinterpret_cast< const double(*)>(_a[5])),(*reinterpret_cast< const double(*)>(_a[6]))); break;
        case 4: _t->onTradeUpdate((*reinterpret_cast< const StrategyId(*)>(_a[1])),(*reinterpret_cast< const PositionId(*)>(_a[2])),(*reinterpret_cast< const double(*)>(_a[3])),(*reinterpret_cast< const double(*)>(_a[4]))); break;
        case 5: _t->removePosition((*reinterpret_cast< const StrategyId(*)>(_a[1])),(*reinterpret_cast< const PositionId(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PositionView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PositionView::staticMetaObject = {
    { &TableView<PositionView>::staticMetaObject, qt_meta_stringdata_PositionView,
      qt_meta_data_PositionView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PositionView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PositionView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PositionView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PositionView))
        return static_cast<void*>(const_cast< PositionView*>(this));
    return TableView<PositionView>::qt_metacast(_clname);
}

int PositionView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TableView<PositionView>::qt_metacall(_c, _id, _a);
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
void PositionView::closed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
