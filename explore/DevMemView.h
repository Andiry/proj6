#ifndef DEVMEMVIEW_H
#define DEVMEMVIEW_H

#include "MemView.h"

class DevMemView : public MemView
{
  Q_OBJECT

  public:
    DevMemView(QWidget *parent);
  private slots:
    void refresh();
};

#endif //SYSMEMVIEW_H

