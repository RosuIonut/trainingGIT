/****************************************************************************
** Meta object code from reading C++ file 'MapDrawerGS.h'
**
** Created: Tue Sep 4 15:46:49 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "headers/MapDrawerGS.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MapDrawerGS.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MapDrawerGS[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   13,   12,   12, 0x05,
      49,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      61,   12,   12,   12, 0x0a,
      76,   12,   12,   12, 0x0a,
      99,   86,   12,   12, 0x0a,
     117,   12,   12,   12, 0x0a,
     127,   13,   12,   12, 0x0a,
     163,  154,   12,   12, 0x0a,
     209,  193,   12,   12, 0x0a,
     253,  237,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MapDrawerGS[] = {
    "MapDrawerGS\0\0direction\0slideMap(E_MoveDirection)\0"
    "dataReady()\0initializeGL()\0paintGL()\0"
    "width,height\0resizeGL(int,int)\0drawMap()\0"
    "moveSlice(E_MoveDirection)\0path,pen\0"
    "renderPath(QPainterPath,QPen)\0"
    "rectangle,brush\0setBackground(QRect,QBrush)\0"
    "x1,y1,x2,y2,pen\0drawLine(int,int,int,int,QPen)\0"
};

const QMetaObject MapDrawerGS::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_MapDrawerGS,
      qt_meta_data_MapDrawerGS, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MapDrawerGS::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MapDrawerGS::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MapDrawerGS::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MapDrawerGS))
        return static_cast<void*>(const_cast< MapDrawerGS*>(this));
    if (!strcmp(_clname, "MapDrawer"))
        return static_cast< MapDrawer*>(const_cast< MapDrawerGS*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int MapDrawerGS::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: slideMap((*reinterpret_cast< E_MoveDirection(*)>(_a[1]))); break;
        case 1: dataReady(); break;
        case 2: initializeGL(); break;
        case 3: paintGL(); break;
        case 4: resizeGL((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: drawMap(); break;
        case 6: moveSlice((*reinterpret_cast< E_MoveDirection(*)>(_a[1]))); break;
        case 7: renderPath((*reinterpret_cast< QPainterPath(*)>(_a[1])),(*reinterpret_cast< QPen(*)>(_a[2]))); break;
        case 8: setBackground((*reinterpret_cast< QRect(*)>(_a[1])),(*reinterpret_cast< QBrush(*)>(_a[2]))); break;
        case 9: drawLine((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< QPen(*)>(_a[5]))); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void MapDrawerGS::slideMap(E_MoveDirection _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MapDrawerGS::dataReady()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
