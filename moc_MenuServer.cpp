/****************************************************************************
** Meta object code from reading C++ file 'MenuServer.h'
**
** Created: Wed Nov 21 15:28:13 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "menu/headers/MenuServer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MenuServer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CMenuServer[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      24,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      32,   12,   12,   12, 0x08,
      52,   12,   12,   12, 0x08,
      69,   12,   12,   12, 0x08,
      87,   12,   12,   12, 0x08,
     109,   12,   12,   12, 0x08,
     129,   12,   12,   12, 0x08,
     149,   12,   12,   12, 0x08,
     172,   12,   12,   12, 0x08,
     194,   12,   12,   12, 0x08,
     211,   12,   12,   12, 0x08,
     234,   12,   12,   12, 0x08,
     253,   12,   12,   12, 0x08,
     267,   12,   12,   12, 0x08,
     287,   12,   12,   12, 0x08,
     308,   12,   12,   12, 0x08,
     324,   12,   12,   12, 0x08,
     346,   12,   12,   12, 0x08,
     369,   12,   12,   12, 0x08,
     383,   12,   12,   12, 0x08,
     407,   12,   12,   12, 0x08,
     425,   12,   12,   12, 0x08,
     444,   12,   12,   12, 0x08,
     460,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CMenuServer[] = {
    "CMenuServer\0\0createMenuButton()\0"
    "destroyMenuButton()\0createMainMenu()\0"
    "destroyMainMenu()\0createSearchAddress()\0"
    "hideSearchAddress()\0showSearchAddress()\0"
    "destroySearchAddress()\0createSearchCountry()\0"
    "setCountryName()\0destroySearchCountry()\0"
    "createSearchCity()\0setCityName()\0"
    "destroySearchCity()\0createSearchStreet()\0"
    "setStreetName()\0destroySearchStreet()\0"
    "createSearchStreetNo()\0setStreetNo()\0"
    "destroySearchStreetNo()\0createSearchPOI()\0"
    "destroySearchPOI()\0hideSearchPOI()\0"
    "showSearchPOI()\0"
};

const QMetaObject CMenuServer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CMenuServer,
      qt_meta_data_CMenuServer, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CMenuServer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CMenuServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CMenuServer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CMenuServer))
        return static_cast<void*>(const_cast< CMenuServer*>(this));
    return QWidget::qt_metacast(_clname);
}

int CMenuServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: createMenuButton(); break;
        case 1: destroyMenuButton(); break;
        case 2: createMainMenu(); break;
        case 3: destroyMainMenu(); break;
        case 4: createSearchAddress(); break;
        case 5: hideSearchAddress(); break;
        case 6: showSearchAddress(); break;
        case 7: destroySearchAddress(); break;
        case 8: createSearchCountry(); break;
        case 9: setCountryName(); break;
        case 10: destroySearchCountry(); break;
        case 11: createSearchCity(); break;
        case 12: setCityName(); break;
        case 13: destroySearchCity(); break;
        case 14: createSearchStreet(); break;
        case 15: setStreetName(); break;
        case 16: destroySearchStreet(); break;
        case 17: createSearchStreetNo(); break;
        case 18: setStreetNo(); break;
        case 19: destroySearchStreetNo(); break;
        case 20: createSearchPOI(); break;
        case 21: destroySearchPOI(); break;
        case 22: hideSearchPOI(); break;
        case 23: showSearchPOI(); break;
        default: ;
        }
        _id -= 24;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
