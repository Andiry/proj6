#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <QApplication>
#include "../module/common.h"
#include "MainDlg.h"

int gfd;
void *gSysAddr;
void *gDevAddr;

int main(int argc, char *argv[])
{
  int res;
  gfd = -1;
  gSysAddr = NULL;
  gDevAddr = NULL;

  gfd = open("/dev/explorer", O_RDWR);
  if (gfd == -1)
  {
    printf("Couldn't open /dev/explorer\n");
    return 0;
  }

// Allocate System memory
  ioctl(gfd, EXP_IOCTALLOCDMABUF, 4);
  ioctl(gfd, EXP_IOCGDMABUFPA, &gSysAddr);

// Map Device memory
  gDevAddr = mmap(NULL, 256*1024*1024, PROT_READ|PROT_WRITE, MAP_SHARED, gfd, (off_t)0x00000000);


  QApplication app(argc, argv);
  MainDlg *pMainDlg = new MainDlg(gfd, gSysAddr, gDevAddr);
  pMainDlg->show();
  res = app.exec();

// Release System memory
  ioctl(gfd, EXP_IOCTFREEDMABUF);

// Unmap Device memory
  munmap(gDevAddr, 256*1024*1024);

  close(gfd);

  return res;
}

