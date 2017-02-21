/****************************************************************************
** Meta object code from reading C++ file 'SearchAddress.h'
**
** Created: Wed Nov 21 15:26:18 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "menu/headers/SearchAddress.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SearchAddress.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CSearchAddress[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      29,   15,   15,   15, 0x05,
      47,   15,   15,   15, 0x05,
      68,   15,   15,   15, 0x05,
      86,   15,   15,   15, 0x05,
     106,   15,   15,   15, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CSearchAddress[] = {
    "CSearchAddress\0\0MapClicked()\0"
    "PreviousClicked()\0ToolCountryClicked()\0"
    "ToolCityClicked()\0ToolStreetClicked()\0"
    "ToolStreetNoClicked()\0"
};

const QMetaObject CSearchAddress::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CSearchAddress,
      qt_meta_data_CSearchAddress, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CSearchAddress::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CSearchAddress::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CSearchAddress::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSearchAddress))
        return static_cast<void*>(const_cast< CSearchAddress*>(this));
    return QWidget::qt_metacast(_clname);
}

int CSearchAddress::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: MapClicked(); break;
        case 1: PreviousClicked(); break;
        case 2: ToolCountryClicked(); break;
        case 3: ToolCityClicked(); break;
        case 4: ToolStreetClicked(); break;
        case 5: ToolStreetNoClicked(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void CSearchAddress::MapClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CSearchAddress::PreviousClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CSearchAddress::ToolCountryClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void CSearchAddress::ToolCityClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void CSearchAddress::ToolStreetClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void CSearchAddress::ToolStreetNoClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}
QT_END_MOC_NAMESPACE
