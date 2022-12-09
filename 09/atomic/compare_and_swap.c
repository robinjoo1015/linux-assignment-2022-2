#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>

#define KTHREAD_COUNT 4

struct task_struct* kthreads[KTHREAD_COUNT];

int counter = 0;
int lock = 0;

static int compare_and_swap(void *data)
{	
	while(!kthread_should_stop()) {
		
		while(__sync_val_compare_and_swap(&lock, 0, 1))
			;
		counter += 1;
		lock = 0;
		
		printk(KERN_INFO "pid[%u] %s: counter: %d\n", current->pid, __func__, counter);
		msleep(500);
	}
	
	do_exit(0);
}

static int __init compare_and_swap_init(void)
{
	printk(KERN_INFO "%s: Entering compare and swap module!\n", __func__);
	kthreads[0] = kthread_run(compare_and_swap, NULL, "thread 0");
	kthreads[1] = kthread_run(compare_and_swap, NULL, "thread 1");
	kthreads[2] = kthread_run(compare_and_swap, NULL, "thread 2");
	kthreads[3] = kthread_run(compare_and_swap, NULL, "thread 3");
	
	return 0;
}

static void __exit compare_and_swap_exit(void)
{
	kthread_stop(kthreads[0]);
	kthread_stop(kthreads[1]);
	kthread_stop(kthreads[2]);
	kthread_stop(kthreads[3]);
	
	printk(KERN_INFO "%s: Exiting compare and swap module!\n", __func__);
}

module_init(compare_and_swap_init);
module_exit(compare_and_swap_exit);
MODULE_LICENSE("GPL");
