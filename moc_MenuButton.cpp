/****************************************************************************
** Meta object code from reading C++ file 'MenuButton.h'
**
** Created: Wed Nov 21 15:26:16 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "menu/headers/MenuButton.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MenuButton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CMenuButton[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CMenuButton[] = {
    "CMenuButton\0\0MenuClicked()\0"
};

const QMetaObject CMenuButton::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CMenuButton,
      qt_meta_data_CMenuButton, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CMenuButton::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CMenuButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CMenuButton::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CMenuButton))
        return static_cast<void*>(const_cast< CMenuButton*>(this));
    return QWidget::qt_metacast(_clname);
}

int CMenuButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: MenuClicked(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void CMenuButton::MenuClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
