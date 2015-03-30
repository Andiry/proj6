#ifndef DCBSETUPDLG_H
#define DCBSETUPDLG_H

#include <QDialog>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>

class DCBSetupDlg : public QDialog
{
  Q_OBJECT

  public:
    DCBSetupDlg(QWidget *parent);

  private slots:
    void readDCB();
    void writeDCB();

  private:
    QLabel *labelStatus;
    QLabel *labelCommand;
    QLabel *labelSysAddr;
    QLabel *labelDevAddr;
    QLabel *labelIterations;
    QLabel *labelSize;
    QTextEdit *textStatus;
    QTextEdit *textCommand;
    QTextEdit *textSysAddr;
    QTextEdit *textDevAddr;
    QTextEdit *textIterations;
    QTextEdit *textSize;

    QLabel *labelOffset;
    QTextEdit *textOffset;
    QPushButton *pushRead;
    QPushButton *pushWrite;
    QPushButton *pushClose;
};

#endif //DCBSETUPDLG_H

