/****************************************************************************
** Meta object code from reading C++ file 'MemInitDlg.h'
**
** Created: Wed Sep 17 15:05:12 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MemInitDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MemInitDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_MemInitDlg[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      20,   11,   11,   11, 0x08,
      34,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MemInitDlg[] = {
    "MemInitDlg\0\0apply()\0selectConst()\0"
    "selectInc()\0"
};

const QMetaObject MemInitDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_MemInitDlg,
      qt_meta_data_MemInitDlg, 0 }
};

const QMetaObject *MemInitDlg::metaObject() const
{
    return &staticMetaObject;
}

void *MemInitDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MemInitDlg))
	return static_cast<void*>(const_cast< MemInitDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int MemInitDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: apply(); break;
        case 1: selectConst(); break;
        case 2: selectInc(); break;
        }
        _id -= 3;
    }
    return _id;
}
