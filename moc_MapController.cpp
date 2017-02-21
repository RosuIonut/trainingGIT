/****************************************************************************
** Meta object code from reading C++ file 'MapController.h'
**
** Created: Wed Nov 21 15:26:12 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "headers/MapController.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MapController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MapController[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      34,   15,   14,   14, 0x05,
      89,   85,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     109,   15,   14,   14, 0x0a,
     142,   15,   14,   14, 0x0a,
     203,  195,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MapController[] = {
    "MapController\0\0latitude,longitude\0"
    "positionChanged(DT_MapCoordinate,DT_MapCoordinate)\0"
    "x,y\0imageSlide(int,int)\0"
    "onPositionChanged(double,double)\0"
    "onPositionChanged(DT_MapCoordinate,DT_MapCoordinate)\0"
    "overlay\0removeOverlay(QObject*)\0"
};

const QMetaObject MapController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MapController,
      qt_meta_data_MapController, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MapController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MapController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MapController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MapController))
        return static_cast<void*>(const_cast< MapController*>(this));
    return QObject::qt_metacast(_clname);
}

int MapController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: positionChanged((*reinterpret_cast< DT_MapCoordinate(*)>(_a[1])),(*reinterpret_cast< DT_MapCoordinate(*)>(_a[2]))); break;
        case 1: imageSlide((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: onPositionChanged((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 3: onPositionChanged((*reinterpret_cast< DT_MapCoordinate(*)>(_a[1])),(*reinterpret_cast< DT_MapCoordinate(*)>(_a[2]))); break;
        case 4: removeOverlay((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void MapController::positionChanged(DT_MapCoordinate _t1, DT_MapCoordinate _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MapController::imageSlide(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
