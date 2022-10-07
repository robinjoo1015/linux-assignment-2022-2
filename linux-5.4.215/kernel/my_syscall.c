#include <linux/syscalls.h>

SYSCALL_DEFINE0(mycall)
{
	printk("System Call by 20184757 YoungSeok Joo!\n");
	
	return 0;
}
