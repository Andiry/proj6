#include <QtGui>

#include "MainDlg.h"
#include "HWInfoDlg.h"
#include "HWTestDlg.h"
#include "MemInitDlg.h"
#include "DCBSetupDlg.h"
#include "StartDMADlg.h"

MainDlg::MainDlg(int fd, void *SysAddr, void *DevAddr, QWidget *parent)
{
  m_fd = fd;
  m_SysAddr = SysAddr;
  m_DevAddr = DevAddr;

    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("MainDlg"));
    this->resize(750, 400);
    this->setWindowTitle("Explore");

    pushButton0 = new QPushButton(this);
    pushButton0->setGeometry(QRect(650, 60, 80, 27));
    pushButton0->setText("HW Info");

    pushButton1 = new QPushButton(this);
    pushButton1->setGeometry(QRect(650, 100, 80, 27));
    pushButton1->setText("HW Test");

    pushButton2 = new QPushButton(this);
    pushButton2->setGeometry(QRect(650, 140, 80, 27));
    pushButton2->setText("Mem Init");

    pushButton3 = new QPushButton(this);
    pushButton3->setGeometry(QRect(650, 180, 80, 27));
    pushButton3->setText("Setup DCB");

    pushButton4 = new QPushButton(this);
    pushButton4->setGeometry(QRect(650, 220, 80, 27));
    pushButton4->setText("Start DMA");

    pushButton5 = new QPushButton(this);
    pushButton5->setGeometry(QRect(650, 260, 80, 27));
    pushButton5->setText("Close");

    pSysMemView = new SysMemView(this);
    pSysMemView->setGeometry(QRect(5, 10, 300, 400));
    pSysMemView->setInfo(m_SysAddr, 64*1024);

    pDevMemView = new DevMemView(this);
    pDevMemView->setGeometry(QRect(305, 10, 300, 400));
    pDevMemView->setInfo(m_DevAddr, 256*1024*1024);

    connect(pushButton0, SIGNAL(clicked()), this, SLOT(showHWInfo()));
    connect(pushButton1, SIGNAL(clicked()), this, SLOT(showHWTest()));
    connect(pushButton2, SIGNAL(clicked()), this, SLOT(showMemInit()));
    connect(pushButton3, SIGNAL(clicked()), this, SLOT(showSetupDCB()));
    connect(pushButton4, SIGNAL(clicked()), this, SLOT(startDMA()));
    connect(pushButton5, SIGNAL(clicked()), this, SLOT(close()));
}

MainDlg::~MainDlg()
{
}

void MainDlg::showHWInfo()
{
  HWInfoDlg *pHWInfoDlg = new HWInfoDlg(m_fd, this);
  pHWInfoDlg->setModal(true);
  pHWInfoDlg->show();
}

void MainDlg::showHWTest()
{
  HWTestDlg *pHWTestDlg = new HWTestDlg(this);
  pHWTestDlg->setModal(true);
  pHWTestDlg->show();
}

void MainDlg::showMemInit()
{
  MemInitDlg *pMemInitDlg = new MemInitDlg(this);
  pMemInitDlg->setModal(this);
  pMemInitDlg->show();
}

void MainDlg::showSetupDCB()
{
  DCBSetupDlg *pDCBSetupDlg = new DCBSetupDlg(this);
  pDCBSetupDlg->setModal(this);
  pDCBSetupDlg->show();
}

void MainDlg::startDMA()
{
  StartDMADlg *pStartDMADlg = new StartDMADlg(this);
  pStartDMADlg->setModal(this);
  pStartDMADlg->show();
}


