#include "SysMemView.h"

#define SYS_MEM_SIZE 64*1024

extern int gfd;


SysMemView::SysMemView(QWidget *parent)
  :MemView(parent)
{
  setTitle("System Memory");
  connect(pushRefresh, SIGNAL(clicked()), this, SLOT(refresh()));
}

void SysMemView::refresh()
{
  unsigned int offset;
  QString s;
  bool ok;

  s = textOffset->toPlainText();
  offset = s.toUInt(&ok, 0);

  unsigned char rd_buf[SYS_MEM_SIZE];

  read(gfd, rd_buf, SYS_MEM_SIZE);

  display(offset, (unsigned char*)(rd_buf + offset));

//  unsigned char buf[128];
//  display(0, buf);
}

