#check gcc version
#$(warning $(shell echo "-------------------------"))
#$(warning "Please check gcc are the same!!")
#$(warning $(shell cat /proc/version))
#$(warning $(shell gcc --version))
#$(warning $(shell echo "-------------------------"))


obj-m := scull.o
KERNEL_DIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)


all:
	make -C $(KERNEL_DIR) M=$(PWD) modules
clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
.PHONY:clean

user:
	gcc user_scull_program.c -o user_scull_program.o
