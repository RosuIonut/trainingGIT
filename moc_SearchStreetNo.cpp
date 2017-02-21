/****************************************************************************
** Meta object code from reading C++ file 'SearchStreetNo.h'
**
** Created: Wed Nov 21 15:26:24 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "menu/headers/SearchStreetNo.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SearchStreetNo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CSearchStreetNo[] = {

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
      17,   16,   16,   16, 0x05,
      35,   16,   16,   16, 0x05,
      49,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      62,   16,   16,   16, 0x08,
      76,   16,   16,   16, 0x08,
      87,   16,   16,   16, 0x08,
      98,   16,   16,   16, 0x08,
     109,   16,   16,   16, 0x08,
     120,   16,   16,   16, 0x08,
     131,   16,   16,   16, 0x08,
     142,   16,   16,   16, 0x08,
     153,   16,   16,   16, 0x08,
     164,   16,   16,   16, 0x08,
     175,   16,   16,   16, 0x08,
     186,   16,   16,   16, 0x08,
     197,   16,   16,   16, 0x08,
     208,   16,   16,   16, 0x08,
     219,   16,   16,   16, 0x08,
     230,   16,   16,   16, 0x08,
     241,   16,   16,   16, 0x08,
     252,   16,   16,   16, 0x08,
     263,   16,   16,   16, 0x08,
     274,   16,   16,   16, 0x08,
     285,   16,   16,   16, 0x08,
     296,   16,   16,   16, 0x08,
     307,   16,   16,   16, 0x08,
     318,   16,   16,   16, 0x08,
     329,   16,   16,   16, 0x08,
     340,   16,   16,   16, 0x08,
     351,   16,   16,   16, 0x08,
     362,   16,   16,   16, 0x08,
     375,   16,   16,   16, 0x08,
     388,   16,   16,   16, 0x08,
     401,   16,   16,   16, 0x08,
     414,   16,   16,   16, 0x08,
     427,   16,   16,   16, 0x08,
     440,   16,   16,   16, 0x08,
     453,   16,   16,   16, 0x08,
     466,   16,   16,   16, 0x08,
     479,   16,   16,   16, 0x08,
     492,   16,   16,   16, 0x08,
     505,   16,   16,   16, 0x08,
     520,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CSearchStreetNo[] = {
    "CSearchStreetNo\0\0PreviousClicked()\0"
    "NextClicked()\0MapClicked()\0SetStreetNo()\0"
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

const QMetaObject CSearchStreetNo::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CSearchStreetNo,
      qt_meta_data_CSearchStreetNo, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CSearchStreetNo::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CSearchStreetNo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CSearchStreetNo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSearchStreetNo))
        return static_cast<void*>(const_cast< CSearchStreetNo*>(this));
    return QWidget::qt_metacast(_clname);
}

int CSearchStreetNo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: PreviousClicked(); break;
        case 1: NextClicked(); break;
        case 2: MapClicked(); break;
        case 3: SetStreetNo(); break;
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
void CSearchStreetNo::PreviousClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CSearchStreetNo::NextClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CSearchStreetNo::MapClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
