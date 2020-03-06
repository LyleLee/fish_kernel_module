#check gcc version
#$(warning $(shell echo "-------------------------"))
#$(warning "Please check gcc are the same!!")
#$(warning $(shell cat /proc/version))
#$(warning $(shell gcc --version))
#$(warning $(shell echo "-------------------------"))


obj-m := useproc.o
KERNEL_DIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)


all:
	make -C $(KERNEL_DIR) M=$(PWD) modules
clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
	rm *.out

.PHONY:clean

user:
	gcc test_proc_module.c -o test_proc_module.out


