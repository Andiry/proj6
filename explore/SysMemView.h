#ifndef SYSMEMVIEW_H
#define SYSMEMVIEW_H

#include "MemView.h"

class SysMemView : public MemView
{
  Q_OBJECT

  public:
    SysMemView(QWidget *parent);
  private slots:
    void refresh();
};

#endif //SYSMEMVIEW_H

