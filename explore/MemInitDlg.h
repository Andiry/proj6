#ifndef MEMINITDLG_H
#define MEMINITDLG_H

#include <QDialog>
#include <QGroupBox>
#include <QRadioButton>
#include <QTextEdit>
#include <QPushButton>

class MemInitDlg : public QDialog
{
  Q_OBJECT

  public:
    MemInitDlg(QWidget *parent);

  private slots:
    void apply();
    void selectConst();
    void selectInc();

  private:
    void DevMemInit(unsigned int type, unsigned int data);
    void SysMemInit(unsigned int type, unsigned int data);
    QGroupBox *groupMem;
    QRadioButton *radioSys;
    QRadioButton *radioDev;
    QGroupBox *groupFill;
    QRadioButton *radioFillConst;
    QRadioButton *radioFillInc;
    QTextEdit *textFillConst;
    QTextEdit *textFillInc;
    QPushButton *pushApply;
    QPushButton *pushClose;
};

#endif //MEMINITDLG_H

