/****************************************************************************
** Meta object code from reading C++ file 'InstrumentView.h'
**
** Created: Tue May 8 23:26:20 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AIMSTrader/Platform/View/InstrumentView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'InstrumentView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_InstrumentView[] = {

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
      16,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      27,   25,   15,   15, 0x0a,
      83,   60,   15,   15, 0x0a,
     136,  129,   15,   15, 0x0a,
     182,  174,   15,   15, 0x0a,
     226,   25,   15,   15, 0x0a,
     262,   25,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_InstrumentView[] = {
    "InstrumentView\0\0closed()\0,\0"
    "addInstrument(TickerId,Contract)\0"
    ",,price,canAutoExecute\0"
    "updateTickPrice(TickerId,TickType,double,int)\0"
    ",,size\0updateTickSize(TickerId,TickType,int)\0"
    ",,value\0updateTickGeneric(TickerId,TickType,double)\0"
    "onTradeUpdate(TickerId,TradeUpdate)\0"
    "onQuoteUpdate(TickerId,QuoteUpdate)\0"
};

void InstrumentView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        InstrumentView *_t = static_cast<InstrumentView *>(_o);
        switch (_id) {
        case 0: _t->closed(); break;
        case 1: _t->addInstrument((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< const Contract(*)>(_a[2]))); break;
        case 2: _t->updateTickPrice((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< const TickType(*)>(_a[2])),(*reinterpret_cast< const double(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4]))); break;
        case 3: _t->updateTickSize((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< const TickType(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 4: _t->updateTickGeneric((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< const TickType(*)>(_a[2])),(*reinterpret_cast< const double(*)>(_a[3]))); break;
        case 5: _t->onTradeUpdate((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< const TradeUpdate(*)>(_a[2]))); break;
        case 6: _t->onQuoteUpdate((*reinterpret_cast< const TickerId(*)>(_a[1])),(*reinterpret_cast< const QuoteUpdate(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData InstrumentView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject InstrumentView::staticMetaObject = {
    { &TableView<InstrumentView>::staticMetaObject, qt_meta_stringdata_InstrumentView,
      qt_meta_data_InstrumentView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &InstrumentView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *InstrumentView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *InstrumentView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_InstrumentView))
        return static_cast<void*>(const_cast< InstrumentView*>(this));
    return TableView<InstrumentView>::qt_metacast(_clname);
}

int InstrumentView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TableView<InstrumentView>::qt_metacall(_c, _id, _a);
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
void InstrumentView::closed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
