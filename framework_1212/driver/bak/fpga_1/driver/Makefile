DRIVER = gpmc_fpga

ifneq ($(KERNELRELEASE),)
    obj-m := $(DRIVER).o
else
    PWD := $(shell pwd)

KERNELDIR=/opt/sdb/linux-2.6.32-CNG205 
FPGA_ARCH=arm
FPGA_CROSS_COMPILE=arm-none-linux-gnueabi-

default:
ifeq ($(strip $(KERNELDIR)),)
	$(error "KERNELDIR is undefined!")
else
	$(MAKE) -C $(KERNELDIR) ARCH=$(FPGA_ARCH) CROSS_COMPILE=$(FPGA_CROSS_COMPILE) M=$(PWD) modules 
endif


clean:
	rm -rf *~ *.ko *.o *.mod.c modules.order Module.symvers .$(DRIVER)* .tmp_versions *.out tags

endif

