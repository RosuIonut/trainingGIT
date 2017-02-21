/****************************************************************************
** Meta object code from reading C++ file 'SearchCountry.h'
**
** Created: Wed Nov 21 15:26:20 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "menu/headers/SearchCountry.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SearchCountry.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CSearchCountry[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      43,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      34,   15,   15,   15, 0x05,
      48,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      61,   15,   15,   15, 0x08,
      78,   15,   15,   15, 0x08,
      89,   15,   15,   15, 0x08,
     100,   15,   15,   15, 0x08,
     111,   15,   15,   15, 0x08,
     122,   15,   15,   15, 0x08,
     133,   15,   15,   15, 0x08,
     144,   15,   15,   15, 0x08,
     155,   15,   15,   15, 0x08,
     166,   15,   15,   15, 0x08,
     177,   15,   15,   15, 0x08,
     188,   15,   15,   15, 0x08,
     199,   15,   15,   15, 0x08,
     210,   15,   15,   15, 0x08,
     221,   15,   15,   15, 0x08,
     232,   15,   15,   15, 0x08,
     243,   15,   15,   15, 0x08,
     254,   15,   15,   15, 0x08,
     265,   15,   15,   15, 0x08,
     276,   15,   15,   15, 0x08,
     287,   15,   15,   15, 0x08,
     298,   15,   15,   15, 0x08,
     309,   15,   15,   15, 0x08,
     320,   15,   15,   15, 0x08,
     331,   15,   15,   15, 0x08,
     342,   15,   15,   15, 0x08,
     353,   15,   15,   15, 0x08,
     364,   15,   15,   15, 0x08,
     377,   15,   15,   15, 0x08,
     390,   15,   15,   15, 0x08,
     403,   15,   15,   15, 0x08,
     416,   15,   15,   15, 0x08,
     429,   15,   15,   15, 0x08,
     442,   15,   15,   15, 0x08,
     455,   15,   15,   15, 0x08,
     468,   15,   15,   15, 0x08,
     481,   15,   15,   15, 0x08,
     494,   15,   15,   15, 0x08,
     507,   15,   15,   15, 0x08,
     522,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CSearchCountry[] = {
    "CSearchCountry\0\0PreviousClicked()\0"
    "NextClicked()\0MapClicked()\0SetCountryName()\0"
    "AClicked()\0BClicked()\0CClicked()\0"
    "DClicked()\0EClicked()\0FClicked()\0"
    "GClicked()\0HClicked()\0IClicked()\0"
    "JClicked()\0KClicked()\0LClicked()\0"
    "MClicked()\0NClicked()\0OClicked()\0"
    "PClicked()\0QClicked()\0RClicked()\0"
    "SClicked()\0TClicked()\0UClicked()\0"
    "VClicked()\0WClicked()\0XClicked()\0"
    "YClicked()\0ZClicked()\0No0Clicked()\0"
    "No1Clicked()\0No2Clicked()\0No3Clicked()\0"
    "No4Clicked()\0No5Clicked()\0No6Clicked()\0"
    "No7Clicked()\0No8Clicked()\0No9Clicked()\0"
    "DelClicked()\0SpaceClicked()\0DashClicked()\0"
};

const QMetaObject CSearchCountry::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CSearchCountry,
      qt_meta_data_CSearchCountry, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CSearchCountry::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CSearchCountry::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CSearchCountry::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSearchCountry))
        return static_cast<void*>(const_cast< CSearchCountry*>(this));
    return QWidget::qt_metacast(_clname);
}

int CSearchCountry::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: PreviousClicked(); break;
        case 1: NextClicked(); break;
        case 2: MapClicked(); break;
        case 3: SetCountryName(); break;
        case 4: AClicked(); break;
        case 5: BClicked(); break;
        case 6: CClicked(); break;
        case 7: DClicked(); break;
        case 8: EClicked(); break;
        case 9: FClicked(); break;
        case 10: GClicked(); break;
        case 11: HClicked(); break;
        case 12: IClicked(); break;
        case 13: JClicked(); break;
        case 14: KClicked(); break;
        case 15: LClicked(); break;
        case 16: MClicked(); break;
        case 17: NClicked(); break;
        case 18: OClicked(); break;
        case 19: PClicked(); break;
        case 20: QClicked(); break;
        case 21: RClicked(); break;
        case 22: SClicked(); break;
        case 23: TClicked(); break;
        case 24: UClicked(); break;
        case 25: VClicked(); break;
        case 26: WClicked(); break;
        case 27: XClicked(); break;
        case 28: YClicked(); break;
        case 29: ZClicked(); break;
        case 30: No0Clicked(); break;
        case 31: No1Clicked(); break;
        case 32: No2Clicked(); break;
        case 33: No3Clicked(); break;
        case 34: No4Clicked(); break;
        case 35: No5Clicked(); break;
        case 36: No6Clicked(); break;
        case 37: No7Clicked(); break;
        case 38: No8Clicked(); break;
        case 39: No9Clicked(); break;
        case 40: DelClicked(); break;
        case 41: SpaceClicked(); break;
        case 42: DashClicked(); break;
        default: ;
        }
        _id -= 43;
    }
    return _id;
}

// SIGNAL 0
void CSearchCountry::PreviousClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CSearchCountry::NextClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CSearchCountry::MapClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
