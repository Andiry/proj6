/****************************************************************************
** Meta object code from reading C++ file 'StartDMADlg.h'
**
** Created: Wed Sep 17 15:05:13 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "StartDMADlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'StartDMADlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_StartDMADlg[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_StartDMADlg[] = {
    "StartDMADlg\0\0startDMA()\0"
};

const QMetaObject StartDMADlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_StartDMADlg,
      qt_meta_data_StartDMADlg, 0 }
};

const QMetaObject *StartDMADlg::metaObject() const
{
    return &staticMetaObject;
}

void *StartDMADlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StartDMADlg))
	return static_cast<void*>(const_cast< StartDMADlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int StartDMADlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: startDMA(); break;
        }
        _id -= 1;
    }
    return _id;
}
