/****************************************************************************
** Meta object code from reading C++ file 'OnScreenKeyboard.h'
**
** Created: Wed Nov 21 15:26:25 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "menu/headers/OnScreenKeyboard.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'OnScreenKeyboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_COnScreenKeyboard[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      39,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      39,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,
      30,   18,   18,   18, 0x05,
      41,   18,   18,   18, 0x05,
      52,   18,   18,   18, 0x05,
      63,   18,   18,   18, 0x05,
      74,   18,   18,   18, 0x05,
      85,   18,   18,   18, 0x05,
      96,   18,   18,   18, 0x05,
     107,   18,   18,   18, 0x05,
     118,   18,   18,   18, 0x05,
     129,   18,   18,   18, 0x05,
     140,   18,   18,   18, 0x05,
     151,   18,   18,   18, 0x05,
     162,   18,   18,   18, 0x05,
     173,   18,   18,   18, 0x05,
     184,   18,   18,   18, 0x05,
     195,   18,   18,   18, 0x05,
     206,   18,   18,   18, 0x05,
     217,   18,   18,   18, 0x05,
     228,   18,   18,   18, 0x05,
     239,   18,   18,   18, 0x05,
     250,   18,   18,   18, 0x05,
     261,   18,   18,   18, 0x05,
     272,   18,   18,   18, 0x05,
     283,   18,   18,   18, 0x05,
     294,   18,   18,   18, 0x05,
     305,   18,   18,   18, 0x05,
     318,   18,   18,   18, 0x05,
     331,   18,   18,   18, 0x05,
     344,   18,   18,   18, 0x05,
     357,   18,   18,   18, 0x05,
     370,   18,   18,   18, 0x05,
     383,   18,   18,   18, 0x05,
     396,   18,   18,   18, 0x05,
     409,   18,   18,   18, 0x05,
     422,   18,   18,   18, 0x05,
     435,   18,   18,   18, 0x05,
     448,   18,   18,   18, 0x05,
     463,   18,   18,   18, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_COnScreenKeyboard[] = {
    "COnScreenKeyboard\0\0AClicked()\0BClicked()\0"
    "CClicked()\0DClicked()\0EClicked()\0"
    "FClicked()\0GClicked()\0HClicked()\0"
    "IClicked()\0JClicked()\0KClicked()\0"
    "LClicked()\0MClicked()\0NClicked()\0"
    "OClicked()\0PClicked()\0QClicked()\0"
    "RClicked()\0SClicked()\0TClicked()\0"
    "UClicked()\0VClicked()\0WClicked()\0"
    "XClicked()\0YClicked()\0ZClicked()\0"
    "No0Clicked()\0No1Clicked()\0No2Clicked()\0"
    "No3Clicked()\0No4Clicked()\0No5Clicked()\0"
    "No6Clicked()\0No7Clicked()\0No8Clicked()\0"
    "No9Clicked()\0DelClicked()\0SpaceClicked()\0"
    "DashClicked()\0"
};

const QMetaObject COnScreenKeyboard::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_COnScreenKeyboard,
      qt_meta_data_COnScreenKeyboard, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &COnScreenKeyboard::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *COnScreenKeyboard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *COnScreenKeyboard::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_COnScreenKeyboard))
        return static_cast<void*>(const_cast< COnScreenKeyboard*>(this));
    return QWidget::qt_metacast(_clname);
}

int COnScreenKeyboard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: AClicked(); break;
        case 1: BClicked(); break;
        case 2: CClicked(); break;
        case 3: DClicked(); break;
        case 4: EClicked(); break;
        case 5: FClicked(); break;
        case 6: GClicked(); break;
        case 7: HClicked(); break;
        case 8: IClicked(); break;
        case 9: JClicked(); break;
        case 10: KClicked(); break;
        case 11: LClicked(); break;
        case 12: MClicked(); break;
        case 13: NClicked(); break;
        case 14: OClicked(); break;
        case 15: PClicked(); break;
        case 16: QClicked(); break;
        case 17: RClicked(); break;
        case 18: SClicked(); break;
        case 19: TClicked(); break;
        case 20: UClicked(); break;
        case 21: VClicked(); break;
        case 22: WClicked(); break;
        case 23: XClicked(); break;
        case 24: YClicked(); break;
        case 25: ZClicked(); break;
        case 26: No0Clicked(); break;
        case 27: No1Clicked(); break;
        case 28: No2Clicked(); break;
        case 29: No3Clicked(); break;
        case 30: No4Clicked(); break;
        case 31: No5Clicked(); break;
        case 32: No6Clicked(); break;
        case 33: No7Clicked(); break;
        case 34: No8Clicked(); break;
        case 35: No9Clicked(); break;
        case 36: DelClicked(); break;
        case 37: SpaceClicked(); break;
        case 38: DashClicked(); break;
        default: ;
        }
        _id -= 39;
    }
    return _id;
}

// SIGNAL 0
void COnScreenKeyboard::AClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void COnScreenKeyboard::BClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void COnScreenKeyboard::CClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void COnScreenKeyboard::DClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void COnScreenKeyboard::EClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void COnScreenKeyboard::FClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void COnScreenKeyboard::GClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void COnScreenKeyboard::HClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void COnScreenKeyboard::IClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}

// SIGNAL 9
void COnScreenKeyboard::JClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}

// SIGNAL 10
void COnScreenKeyboard::KClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 10, 0);
}

// SIGNAL 11
void COnScreenKeyboard::LClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}

// SIGNAL 12
void COnScreenKeyboard::MClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}

// SIGNAL 13
void COnScreenKeyboard::NClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}

// SIGNAL 14
void COnScreenKeyboard::OClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 14, 0);
}

// SIGNAL 15
void COnScreenKeyboard::PClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 15, 0);
}

// SIGNAL 16
void COnScreenKeyboard::QClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 16, 0);
}

// SIGNAL 17
void COnScreenKeyboard::RClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 17, 0);
}

// SIGNAL 18
void COnScreenKeyboard::SClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 18, 0);
}

// SIGNAL 19
void COnScreenKeyboard::TClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 19, 0);
}

// SIGNAL 20
void COnScreenKeyboard::UClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 20, 0);
}

// SIGNAL 21
void COnScreenKeyboard::VClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 21, 0);
}

// SIGNAL 22
void COnScreenKeyboard::WClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 22, 0);
}

// SIGNAL 23
void COnScreenKeyboard::XClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 23, 0);
}

// SIGNAL 24
void COnScreenKeyboard::YClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 24, 0);
}

// SIGNAL 25
void COnScreenKeyboard::ZClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 25, 0);
}

// SIGNAL 26
void COnScreenKeyboard::No0Clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 26, 0);
}

// SIGNAL 27
void COnScreenKeyboard::No1Clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 27, 0);
}

// SIGNAL 28
void COnScreenKeyboard::No2Clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 28, 0);
}

// SIGNAL 29
void COnScreenKeyboard::No3Clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 29, 0);
}

// SIGNAL 30
void COnScreenKeyboard::No4Clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 30, 0);
}

// SIGNAL 31
void COnScreenKeyboard::No5Clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 31, 0);
}

// SIGNAL 32
void COnScreenKeyboard::No6Clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 32, 0);
}

// SIGNAL 33
void COnScreenKeyboard::No7Clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 33, 0);
}

// SIGNAL 34
void COnScreenKeyboard::No8Clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 34, 0);
}

// SIGNAL 35
void COnScreenKeyboard::No9Clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 35, 0);
}

// SIGNAL 36
void COnScreenKeyboard::DelClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 36, 0);
}

// SIGNAL 37
void COnScreenKeyboard::SpaceClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 37, 0);
}

// SIGNAL 38
void COnScreenKeyboard::DashClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 38, 0);
}
QT_END_MOC_NAMESPACE
