#ifndef STARTDMADLG_H
#define STARTDMADLG_H

#include <QDialog>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>

class StartDMADlg : public QDialog
{
  Q_OBJECT

  public:
    StartDMADlg(QWidget *parent);

  private slots:
    void startDMA();

  private:
    QLabel *labelDCBStartAddr;
    QTextEdit *textDCBStartAddr;
    QPushButton *pushOK;
};

#endif //STARTDMADLG_H

