#ifndef MEMVIEW_H
#define MEMVIEW_H
#include <QtGui/QLabel>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>
#include <QPushButton>

class MemView : public QWidget
{
  Q_OBJECT

  public:
    MemView(QWidget *parent);
    void setTitle(const QString &);
    void setInfo(void *start, unsigned int size);
    void display(unsigned int offset, unsigned char *buf);
    void virtual refresh()=0;

  private:
    QLabel *labelTitle;
    QLabel *labelInfo;
    QTextEdit *textMain;
    QLabel *labelOffset;
  protected:
    QTextEdit *textOffset;
    QPushButton *pushRefresh;
};

#endif //MEMVIEW_H

