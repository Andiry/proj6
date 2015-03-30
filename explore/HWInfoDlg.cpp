#include "HWInfoDlg.h"
#include <sys/ioctl.h>
#include "../module/common.h"

HWInfoDlg::HWInfoDlg(int fd, QWidget *parent)
{
  m_fd = fd;

  if (this->objectName().isEmpty())
    this->setObjectName(QString::fromUtf8("HWInfoDlg"));
  this->resize(600, 800);
  this->setWindowTitle("HW Info");

  textMain = new QTextEdit(this);
  textMain->setGeometry(QRect(20, 20, 560, 710));

  pushRefresh = new QPushButton(this);
  pushRefresh->setGeometry(QRect(150, 750, 80, 27));
  pushRefresh->setText("Refresh");

  pushClose = new QPushButton(this);
  pushClose->setGeometry(QRect(370, 750, 80, 27));
  pushClose->setText("Close");

  connect(pushRefresh, SIGNAL(clicked()), this, SLOT(refresh()));
  connect(pushClose, SIGNAL(clicked()), this, SLOT(close()));
}

void HWInfoDlg::LoadHWInfo()
{
  unsigned int datum;
  datum = 0x0;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  m_strHWInfo = "Device ID = 0x" + QString::number((datum >> 16), 16).toUpper() + "  ";
  m_strHWInfo += "Vendor ID = 0x" + QString::number((datum & 0xFFFF), 16).toUpper() + "\n";

  datum = 0x4;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  m_strHWInfo += "Status = 0x" + QString::number((datum >> 16), 16).toUpper() + "  ";
  m_strHWInfo += "Command = 0x" + QString::number((datum & 0xFFFF), 16).toUpper() + "\n";

  datum = 0x8;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  m_strHWInfo += "Class Code = 0x" + QString::number((datum >> 8), 16).toUpper() + "  ";
  m_strHWInfo += "Revision ID = 0x" + QString::number((datum & 0xFF), 16).toUpper() + "\n";

  datum = 0xC;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  m_strHWInfo += "BIST = 0x" + QString::number(((datum & 0xFF000000) >> 24), 16).toUpper() + "  ";
  m_strHWInfo += "Header Type = 0x" + QString::number(((datum & 0x00FF0000) >> 16), 16).toUpper() + "  ";
  m_strHWInfo += "Master Latency Timer = 0x" + QString::number(((datum & 0x0000FF00) >> 8), 16).toUpper() + "  ";
  m_strHWInfo += "Cache Line Size = 0x" + QString::number(((datum & 0x000000FF) >> 0), 16).toUpper() + "\n";
  
  datum = 0x10;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  m_strHWInfo += "BAR0 = 0x" + QString::number(datum, 16).toUpper() + "\n";
  
  datum = 0x14;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  m_strHWInfo += "BAR1 = 0x" + QString::number(datum, 16).toUpper() + "\n";

  datum = 0x18;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  m_strHWInfo += "BAR2 = 0x" + QString::number(datum, 16).toUpper() + "\n";

  datum = 0x1C;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  m_strHWInfo += "BAR3 = 0x" + QString::number(datum, 16).toUpper() + "\n";
 
  datum = 0x20;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  m_strHWInfo += "BAR4 = 0x" + QString::number(datum, 16).toUpper() + "\n";

  datum = 0x24;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  m_strHWInfo += "BAR5 = 0x" + QString::number(datum, 16).toUpper() + "\n";

  datum = 0x28;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  m_strHWInfo += "Cardbus CIS Pointer = 0x" + QString::number(datum, 16).toUpper() + "\n";

  datum = 0x2C;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  m_strHWInfo += "Subsystem ID = 0x" + QString::number(((datum & 0xFFFF0000) >> 16), 16).toUpper() + "  ";
  m_strHWInfo += "Subsystem Vendor ID = 0x" + QString::number(((datum & 0x0000FFFF) >> 0), 16).toUpper() + " \n";

  datum = 0x30;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  m_strHWInfo += "Expansion ROM Base Address = 0x" + QString::number(datum, 16).toUpper() + "\n";
}

void HWInfoDlg::refresh()
{
  LoadHWInfo();
  textMain->setPlainText(m_strHWInfo);
}

