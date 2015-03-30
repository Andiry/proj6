#ifndef HWINFODLG_H
#define HWINFODLG_H
#include <QDialog>
#include <QPushButton>
#include <QTextEdit>

class HWInfoDlg : public QDialog
{
  Q_OBJECT

  public:
    HWInfoDlg(int fd, QWidget *parent);

  private slots:
    void refresh();

  private:
    void LoadHWInfo();
    QTextEdit *textMain;
    QPushButton *pushRefresh;
    QPushButton *pushClose;
    QString m_strHWInfo;
    int m_fd;
};
#endif //HWINFODLG_H
