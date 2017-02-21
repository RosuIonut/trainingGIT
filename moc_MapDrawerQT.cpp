/****************************************************************************
** Meta object code from reading C++ file 'MapDrawerQT.h'
**
** Created: Tue Sep 4 15:46:45 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "headers/MapDrawerQT.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MapDrawerQT.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MapDrawerQT[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   13,   12,   12, 0x05,
      54,   49,   12,   12, 0x05,
      74,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      86,   12,   12,   12, 0x0a,
     105,   96,   12,   12, 0x0a,
     151,  135,   12,   12, 0x0a,
     195,  179,   12,   12, 0x0a,
     226,   13,   12,   12, 0x0a,
     253,   49,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MapDrawerQT[] = {
    "MapDrawerQT\0\0direction\0slideMap(E_MoveDirection)\0"
    "zoom\0zoomMap(E_ZoomType)\0dataReady()\0"
    "drawMap()\0path,pen\0renderPath(QPainterPath,QPen)\0"
    "rectangle,brush\0setBackground(QRect,QBrush)\0"
    "x1,y1,x2,y2,pen\0drawLine(int,int,int,int,QPen)\0"
    "moveSlice(E_MoveDirection)\0"
    "zoomSlice(E_ZoomType)\0"
};

const QMetaObject MapDrawerQT::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MapDrawerQT,
      qt_meta_data_MapDrawerQT, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MapDrawerQT::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MapDrawerQT::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MapDrawerQT::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MapDrawerQT))
        return static_cast<void*>(const_cast< MapDrawerQT*>(this));
    if (!strcmp(_clname, "MapDrawer"))
        return static_cast< MapDrawer*>(const_cast< MapDrawerQT*>(this));
    return QWidget::qt_metacast(_clname);
}

int MapDrawerQT::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: slideMap((*reinterpret_cast< E_MoveDirection(*)>(_a[1]))); break;
        case 1: zoomMap((*reinterpret_cast< E_ZoomType(*)>(_a[1]))); break;
        case 2: dataReady(); break;
        case 3: drawMap(); break;
        case 4: renderPath((*reinterpret_cast< QPainterPath(*)>(_a[1])),(*reinterpret_cast< QPen(*)>(_a[2]))); break;
        case 5: setBackground((*reinterpret_cast< QRect(*)>(_a[1])),(*reinterpret_cast< QBrush(*)>(_a[2]))); break;
        case 6: drawLine((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< QPen(*)>(_a[5]))); break;
        case 7: moveSlice((*reinterpret_cast< E_MoveDirection(*)>(_a[1]))); break;
        case 8: zoomSlice((*reinterpret_cast< E_ZoomType(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void MapDrawerQT::slideMap(E_MoveDirection _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MapDrawerQT::zoomMap(E_ZoomType _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MapDrawerQT::dataReady()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
