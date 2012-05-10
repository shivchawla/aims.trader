/****************************************************************************
** Meta object code from reading C++ file 'OpenOrderView.h'
**
** Created: Tue May 8 23:26:27 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AIMSTrader/Platform/View/OpenOrderView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'OpenOrderView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_OpenOrderView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      26,   24,   14,   14, 0x0a,
      73,   69,   14,   14, 0x0a,
     113,   14,   14,   14, 0x0a,
     134,   24,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_OpenOrderView[] = {
    "OpenOrderView\0\0closed()\0,\0"
    "onExecutionUpdate(OrderId,ExecutionStatus)\0"
    ",,,\0addOrder(OrderId,Order,Contract,String)\0"
    "removeOrder(OrderId)\0"
    "onStatusUpdate(OrderId,ExecutionStatus)\0"
};

void OpenOrderView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        OpenOrderView *_t = static_cast<OpenOrderView *>(_o);
        switch (_id) {
        case 0: _t->closed(); break;
        case 1: _t->onExecutionUpdate((*reinterpret_cast< const OrderId(*)>(_a[1])),(*reinterpret_cast< const ExecutionStatus(*)>(_a[2]))); break;
        case 2: _t->addOrder((*reinterpret_cast< const OrderId(*)>(_a[1])),(*reinterpret_cast< const Order(*)>(_a[2])),(*reinterpret_cast< const Contract(*)>(_a[3])),(*reinterpret_cast< const String(*)>(_a[4]))); break;
        case 3: _t->removeOrder((*reinterpret_cast< const OrderId(*)>(_a[1]))); break;
        case 4: _t->onStatusUpdate((*reinterpret_cast< const OrderId(*)>(_a[1])),(*reinterpret_cast< const ExecutionStatus(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData OpenOrderView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OpenOrderView::staticMetaObject = {
    { &TableView<OpenOrderView>::staticMetaObject, qt_meta_stringdata_OpenOrderView,
      qt_meta_data_OpenOrderView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OpenOrderView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OpenOrderView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OpenOrderView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OpenOrderView))
        return static_cast<void*>(const_cast< OpenOrderView*>(this));
    return TableView<OpenOrderView>::qt_metacast(_clname);
}

int OpenOrderView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TableView<OpenOrderView>::qt_metacall(_c, _id, _a);
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
void OpenOrderView::closed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
