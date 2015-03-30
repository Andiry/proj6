#ifndef COMMON_H
#define COMMON_H

#include <linux/ioctl.h>

#define EXP_IOC_MAGIC 'E'

#define EXP_IOCTALLOCDMABUF  	_IOW(EXP_IOC_MAGIC, 1, int)
#define EXP_IOCGDMABUFPA	    _IOR(EXP_IOC_MAGIC, 2, int)
#define EXP_IOCTFREEDMABUF	  _IO(EXP_IOC_MAGIC, 3)
#define EXP_IOCTSTARTDMA	    _IOW(EXP_IOC_MAGIC, 4, void*)
#define EXP_IOCXCFGDATA		    _IOWR(EXP_IOC_MAGIC, 5, int)
#define EXP_IOCTINSERR		    _IOW(EXP_IOC_MAGIC, 6, int)

#define EXP_IOC_MAXNR 6

#endif //COMMON_H
