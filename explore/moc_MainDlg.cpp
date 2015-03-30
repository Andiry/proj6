/****************************************************************************
** Meta object code from reading C++ file 'MainDlg.h'
**
** Created: Wed Sep 17 15:05:11 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MainDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_MainDlg[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x08,
      22,    8,    8,    8, 0x08,
      35,    8,    8,    8, 0x08,
      49,    8,    8,    8, 0x08,
      64,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainDlg[] = {
    "MainDlg\0\0showHWInfo()\0showHWTest()\0"
    "showMemInit()\0showSetupDCB()\0startDMA()\0"
};

const QMetaObject MainDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_MainDlg,
      qt_meta_data_MainDlg, 0 }
};

const QMetaObject *MainDlg::metaObject() const
{
    return &staticMetaObject;
}

void *MainDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainDlg))
	return static_cast<void*>(const_cast< MainDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int MainDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: showHWInfo(); break;
        case 1: showHWTest(); break;
        case 2: showMemInit(); break;
        case 3: showSetupDCB(); break;
        case 4: startDMA(); break;
        }
        _id -= 5;
    }
    return _id;
}
