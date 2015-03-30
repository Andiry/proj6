#include "MemInitDlg.h"

#include <QVBoxLayout>
#include <QApplication>

#define SYS_MEM_SIZE 64*1024
#define DEV_MEM_SIZE 256*1024*1024

extern int gfd;
extern void *gDevAddr;

MemInitDlg::MemInitDlg(QWidget *parent)
  :QDialog(parent)
{
  this->resize(400, 300);

  groupMem = new QGroupBox("Memory", this);
  groupMem->setGeometry(QRect(20,20, 100, 100));
  
  radioSys = new QRadioButton("System", this);
  radioDev = new QRadioButton("Device", this);

  radioSys->setChecked(true);

  QVBoxLayout *vboxMem = new QVBoxLayout;
  vboxMem->addWidget(radioSys);
  vboxMem->addWidget(radioDev);

  groupMem->setLayout(vboxMem);

  groupFill = new QGroupBox("Patern", this);
  groupFill->setGeometry(QRect(150, 20, 250, 100));
  
  radioFillConst = new QRadioButton("Constant", this);
  radioFillInc = new QRadioButton("Incremental", this);
  
  textFillConst = new QTextEdit(this);
  textFillConst->setGeometry(QRect(280, 50, 100, 25));

  textFillInc = new QTextEdit(this);
  textFillInc->setGeometry(QRect(280, 80, 100, 25));
  textFillInc->setEnabled(false);

  radioFillConst->setChecked(true);

  QVBoxLayout *vboxFill = new QVBoxLayout;
  vboxFill->addWidget(radioFillConst);
  vboxFill->addWidget(radioFillInc);

  groupFill->setLayout(vboxFill);

  pushApply = new QPushButton("Apply", this);
  pushApply->setGeometry(QRect(50, 150, 80, 27));

  pushClose = new QPushButton("Close", this);
  pushClose->setGeometry(QRect(250, 150, 80, 27));

  connect(pushClose, SIGNAL(clicked()), this, SLOT(close()));
  connect(pushApply, SIGNAL(clicked()), this, SLOT(apply()));
  connect(radioFillConst, SIGNAL(clicked()), this, SLOT(selectConst()));
  connect(radioFillInc, SIGNAL(clicked()), this, SLOT(selectInc()));
}

void MemInitDlg::DevMemInit(unsigned int type, unsigned int data)
{
  unsigned int *addr;

  addr = (unsigned int*)gDevAddr;
  for(int i=0; i<DEV_MEM_SIZE/4; i++)
     switch(type)
     {
       case 0:
          *addr++ = data;
          break;
       case 1:
          *addr++ = data + i;
     }
}

void MemInitDlg::SysMemInit(unsigned int type, unsigned int data)
{
  unsigned int buf[SYS_MEM_SIZE/4];

  for(int i=0; i<SYS_MEM_SIZE/4; i++)
    switch(type)
    {
      case 0: 
        buf[i] = data;
        break;
      case 1: 
        buf[i] = data + i;
        break;
    }
  write(gfd, (unsigned char*)buf, SYS_MEM_SIZE);       
}

void MemInitDlg::apply()
{
//  QCursor cur;
//  cur = cursor();
//  setCursor(Qt::BusyCursor);
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
 
  unsigned int type;
  type = (radioFillConst->isChecked())? 0:1;

  unsigned int data;
  QString s;
  bool ok;
  switch(type)
  {
    case 0:
       s = textFillConst->toPlainText();
       break;
    case 1:
       s = textFillInc->toPlainText();
       break;
  }

  data = s.toUInt(&ok, 0);
  if(radioSys->isChecked())
    SysMemInit(type, data);
  else
    DevMemInit(type, data);
//  setCursor(cur);
  QApplication::restoreOverrideCursor();
}

void MemInitDlg::selectConst()
{
  textFillInc->setEnabled(false);
  textFillConst->setEnabled(true);
}

void MemInitDlg::selectInc()
{
  textFillInc->setEnabled(true);
  textFillConst->setEnabled(false);
}
