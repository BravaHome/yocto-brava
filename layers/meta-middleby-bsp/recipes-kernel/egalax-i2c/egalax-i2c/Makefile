####################################################################
DEBUG = y

# Add your debugging flag (or not) to CFLAGS
ifeq ($(DEBUG),y)
  DEBFLAGS = -O -g -DDEBUG # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif

CFLAGS_MODULE = -Wall -pipe $(DEBFLAGS) -I.. #-DVERBOSE -DMODVERSIONS
 
####################################################################

ifneq ($(KERNELRELEASE),)
# call from kernel build system
obj-m	:=	egalax_i2c.o
else
KERNEL_SRC ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)

default:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) modules

modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) modules_install
endif

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.order *.symvers Module.markers
depend .depend dep:
	$(CC) $(CFLAGS) -M *.c > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif

