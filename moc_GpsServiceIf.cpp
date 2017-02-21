/****************************************************************************
** Meta object code from reading C++ file 'GpsServiceIf.h'
**
** Created: Wed Nov 21 15:26:14 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "headers/GpsServiceIf.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GpsServiceIf.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GpsServiceIf[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      33,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      91,   13,   67,   13, 0x0a,
     135,   13,  103,   13, 0x0a,
     171,   13,  145,   13, 0x0a,
     185,   13,  145,   13, 0x0a,
     200,   13,  145,   13, 0x0a,
     231,   13,  211,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GpsServiceIf[] = {
    "GpsServiceIf\0\0latitude,longitude\0"
    "coordinatesChanged(double,double)\0"
    "QDBusPendingReply<uint>\0getCourse()\0"
    "QDBusPendingReply<QVariantList>\0"
    "getData()\0QDBusPendingReply<double>\0"
    "getLatitude()\0getLongitude()\0getSpeed()\0"
    "QDBusPendingReply<>\0refreshGpsData()\0"
};

const QMetaObject GpsServiceIf::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_GpsServiceIf,
      qt_meta_data_GpsServiceIf, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GpsServiceIf::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GpsServiceIf::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GpsServiceIf::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GpsServiceIf))
        return static_cast<void*>(const_cast< GpsServiceIf*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int GpsServiceIf::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: coordinatesChanged((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: { QDBusPendingReply<uint> _r = getCourse();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<uint>*>(_a[0]) = _r; }  break;
        case 2: { QDBusPendingReply<QVariantList> _r = getData();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<QVariantList>*>(_a[0]) = _r; }  break;
        case 3: { QDBusPendingReply<double> _r = getLatitude();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<double>*>(_a[0]) = _r; }  break;
        case 4: { QDBusPendingReply<double> _r = getLongitude();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<double>*>(_a[0]) = _r; }  break;
        case 5: { QDBusPendingReply<double> _r = getSpeed();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<double>*>(_a[0]) = _r; }  break;
        case 6: { QDBusPendingReply<> _r = refreshGpsData();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void GpsServiceIf::coordinatesChanged(double _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
