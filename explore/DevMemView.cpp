#include "DevMemView.h"

extern void *gDevAddr;

DevMemView::DevMemView(QWidget *parent)
  :MemView(parent)
{
  setTitle("Device Memory");
  connect(pushRefresh, SIGNAL(clicked()), this, SLOT(refresh()));
}

void DevMemView::refresh()
{
  unsigned int *addr;
  unsigned int offset;
  QString s;
  bool ok;

  s = textOffset->toPlainText();
  offset = s.toUInt(&ok, 0);

  addr = (unsigned int *)((unsigned long)gDevAddr + offset);

  unsigned int data[32];
  for (int i=0; i<32; i++)
    data[i] = *addr++;
  display(offset, (unsigned char*)data);
}

