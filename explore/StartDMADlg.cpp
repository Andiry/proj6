#include "StartDMADlg.h"
#include <sys/ioctl.h>
#include "../module/common.h"

extern int gfd;

StartDMADlg::StartDMADlg(QWidget *parent)
  :QDialog(parent)
{
  labelDCBStartAddr = new QLabel("DCB Address", this);
  labelDCBStartAddr->setGeometry(QRect(20, 20, 100, 27));

  textDCBStartAddr = new QTextEdit("0xC0000000", this);
  textDCBStartAddr->setGeometry(QRect(20, 50, 100, 27));
  
  pushOK = new QPushButton("OK", this);
  pushOK->setGeometry(QRect(40, 90, 60, 27));

  connect(pushOK, SIGNAL(clicked()), this, SLOT(startDMA()));
}

void StartDMADlg::startDMA()
{
  QString s;
  s = textDCBStartAddr->toPlainText();
  
  unsigned long addr;
  bool ok;

  addr = s.toULong(&ok, 0);

  ioctl(gfd, EXP_IOCTSTARTDMA, addr);
  close();
}

