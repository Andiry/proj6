#include "DCBSetupDlg.h"

extern void *gDevAddr;

DCBSetupDlg::DCBSetupDlg(QWidget *parent)
  :QDialog(parent)
{
  labelStatus = new QLabel(this);
  labelStatus->setGeometry(QRect(20, 20, 100, 27));
  labelStatus->setText("Status");

  labelCommand = new QLabel(this);
  labelCommand->setGeometry(QRect(20, 50, 100, 27));
  labelCommand->setText("Command");

  labelSysAddr = new QLabel(this);
  labelSysAddr->setGeometry(QRect(20, 80, 100, 27));
  labelSysAddr->setText("System Address");

  labelDevAddr = new QLabel(this);
  labelDevAddr->setGeometry(QRect(20, 110, 100, 27));
  labelDevAddr->setText("Internal Address");

  labelIterations = new QLabel(this);
  labelIterations->setGeometry(QRect(20, 140, 100, 27));
  labelIterations->setText("Iterations");

  labelSize = new QLabel(this);
  labelSize->setGeometry(QRect(20, 170, 100, 27));
  labelSize->setText("Transaction Size");

  textStatus = new QTextEdit(this);
  textStatus->setGeometry(QRect(130, 20, 100, 27));
  textStatus->setEnabled(false);

  textCommand = new QTextEdit("0",this);
  textCommand->setGeometry(QRect(130, 50, 100, 27));

  textSysAddr = new QTextEdit(this);
  textSysAddr->setGeometry(QRect(130, 80, 100, 27));

  textDevAddr = new QTextEdit(this);
  textDevAddr->setGeometry(QRect(130, 110, 100, 27));

  textIterations = new QTextEdit("1", this);
  textIterations->setGeometry(QRect(130, 140, 100, 27));

  textSize = new QTextEdit(this);
  textSize->setGeometry(QRect(130, 170, 100, 27));

  labelOffset = new QLabel(this);
  labelOffset->setGeometry(QRect(270, 20, 80, 27));
  labelOffset->setText("Offset");

  textOffset = new QTextEdit("0",this);
  textOffset->setGeometry(QRect(270, 50, 80, 25));

  pushRead = new QPushButton(this);
  pushRead->setGeometry(QRect(270, 80, 80, 25));
  pushRead->setText("Read");

  pushWrite = new QPushButton(this);
  pushWrite->setGeometry(QRect(270, 110, 80, 25));
  pushWrite->setText("Write");

  pushClose = new QPushButton(this);
  pushClose->setGeometry(QRect(270, 170, 80, 25));
  pushClose->setText("Close");

  connect(pushRead, SIGNAL(clicked()), this, SLOT(readDCB()));
  connect(pushWrite, SIGNAL(clicked()), this, SLOT(writeDCB()));
  connect(pushClose, SIGNAL(clicked()), this, SLOT(close()));
}

void DCBSetupDlg::readDCB()
{
  unsigned int *addr;
  unsigned int data;
  char str[32];

  QString s;
  bool ok;

  s = textOffset->toPlainText();
  unsigned int offset = 0;
  offset = s.toUInt(&ok, 16);
  addr = (unsigned int *)((unsigned long)gDevAddr + offset);

  data = *addr;
  sprintf(str, "0x%08X", data); 
  textStatus->setText(str);
  
  addr++;
  data = *addr;
  sprintf(str, "0x%08X", data);
  textCommand->setText(str);

  addr++;  //skip the hihger 32 bits
  addr++;
  data = *addr;
  sprintf(str, "0x%08X", data);
  textSysAddr->setText(str);
  
  addr++;
  addr++;
  data = *addr;
  sprintf(str, "0x%08X", data);
  textDevAddr->setText(str);

  addr++;
  data = *addr;
  sprintf(str, "%d", data);
  textIterations->setText(str);

  addr++;
  data = *addr;
  sprintf(str, "%d", data);
  textSize->setText(str);
}

void DCBSetupDlg::writeDCB()
{
  unsigned int *addr;
  unsigned int data;
  unsigned int offset;
  QString s;
  bool ok;

  s = textOffset->toPlainText();
  offset = s.toUInt(&ok, 0);
  addr = (unsigned int *)((unsigned long)gDevAddr + offset);

  addr++; //for status
  
  s = textCommand->toPlainText();
  data = s.toUInt(&ok, 0);
  *addr++ = data;

  *addr++ = 0; //System Address upper 32 bits

  s = textSysAddr->toPlainText();
  data = s.toUInt(&ok, 0);
  *addr++ = data;

  *addr++ = 0; //Device Address upper 32 bits

  s = textDevAddr->toPlainText();
  data = s.toUInt(&ok, 0);
  *addr++ = data;

  s = textIterations->toPlainText();
  data = s.toUInt(&ok, 0);
  *addr++ = data;

  s = textSize->toPlainText();
  data = s.toUInt(&ok, 0);
  *addr++ = data;
}


