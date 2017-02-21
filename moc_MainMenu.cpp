/****************************************************************************
** Meta object code from reading C++ file 'MainMenu.h'
**
** Created: Wed Nov 21 15:26:15 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "menu/headers/MainMenu.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainMenu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CMainMenu[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,
      24,   10,   10,   10, 0x05,
      41,   10,   10,   10, 0x05,
      54,   10,   10,   10, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CMainMenu[] = {
    "CMainMenu\0\0PoiClicked()\0AddressClicked()\0"
    "MapClicked()\0SettingsClicked()\0"
};

const QMetaObject CMainMenu::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CMainMenu,
      qt_meta_data_CMainMenu, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CMainMenu::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CMainMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CMainMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CMainMenu))
        return static_cast<void*>(const_cast< CMainMenu*>(this));
    return QWidget::qt_metacast(_clname);
}

int CMainMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: PoiClicked(); break;
        case 1: AddressClicked(); break;
        case 2: MapClicked(); break;
        case 3: SettingsClicked(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void CMainMenu::PoiClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CMainMenu::AddressClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CMainMenu::MapClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void CMainMenu::SettingsClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
