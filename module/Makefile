

# TARGET = expdrv


ifneq ($(KERNELRELEASE),)

obj-m := expdrv.o
expdrv-objs := expmain.o expdma.o

else

	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

endif

clean:
	rm -rf *.o *.ko *~ *.mod.c .exp* Module.symvers
