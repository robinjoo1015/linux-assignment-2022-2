obj-m 		:= fetch_and_add.o test_and_set.o compare_and_swap.o

KERNEL_DIR 	:= /lib/modules/$(shell uname -r)/build
PWD 		:= $(shell pwd)

default:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules
	
clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
