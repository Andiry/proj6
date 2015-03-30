#include "MemView.h"

MemView::MemView(QWidget *parent)
  :QWidget(parent)
{
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("MemView"));
    this->resize(300, 400);

    labelTitle = new QLabel(this);
    labelTitle->setGeometry(QRect(10, 10, 300, 15));

    labelInfo = new QLabel(this);
    labelInfo->setGeometry(QRect(10, 30, 300, 15));

    textMain = new QTextEdit(this);
    textMain->setGeometry(QRect(10, 50, 300, 280));
    QFont font;
    font.setFixedPitch(true);
    font.setStyleHint(QFont::TypeWriter, QFont::PreferMatch);
    font.setPointSize(9);
    textMain->setFont(font);
    textMain->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    labelOffset = new QLabel(this);
    labelOffset->setGeometry(QRect(10, 340, 100, 27));
    labelOffset->setText("Offset:");

    textOffset = new QTextEdit(this);
    textOffset->setGeometry(QRect(60, 340, 100, 27));
    textOffset->setPlainText("0x00000000");
    
    pushRefresh = new QPushButton(this);
    pushRefresh->setGeometry(QRect(200, 340, 80, 27));
    pushRefresh->setText("Refresh");

}

void MemView::setTitle(const QString &s)
{
  labelTitle->setText(s);
}

void MemView::setInfo(void *start, unsigned int size)
{
  QString s;
  s = "Start: 0x" + QString::number((unsigned long)start, 16).toUpper() + "  Size: " + QString::number(size);
  labelInfo->setText(s);
}

void MemView::display(unsigned int offset, unsigned char *buf)
{
  QString s;
  int i;
  char line[40];

  for (i = 0; i<16; i++)
  {
    sprintf(line, "0x%08X: %02X %02X %02X %02X %02X %02X %02X %02X\n", offset + 8*i,
        buf[8*i+0], buf[8*i+1], buf[8*i+2], buf[8*i+3], buf[8*i+4], buf[8*i+5], buf[8*i+6], buf[8*i+7]);
    s += line;
  }
  textMain->setPlainText(s);
}

