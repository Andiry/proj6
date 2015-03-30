#ifndef MAINDLG_H
#define MAINDLG_H

#include <QDialog>
#include <QtGui/QPushButton>
#include "SysMemView.h"
#include "DevMemView.h"

class MainDlg : public QDialog
{
  Q_OBJECT

  public:
    MainDlg(int fd, void *SysAddr, void *DevAddr, QWidget *parent = 0);
    ~MainDlg();

  private slots:
    void showHWInfo();
    void showHWTest();
    void showMemInit();
    void showSetupDCB();
    void startDMA();

  private:
    QPushButton *pushButton0, *pushButton1, *pushButton2, *pushButton3, *pushButton4, *pushButton5;
    SysMemView *pSysMemView;
    DevMemView *pDevMemView;
    int m_fd;
    void *m_SysAddr;
    void *m_DevAddr;
};

#endif //MAINDLG_H
