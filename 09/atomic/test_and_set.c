#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>

#define KTHREAD_COUNT 4

struct task_struct* kthreads[KTHREAD_COUNT];

int counter = 0;
int lock = 0;

static int test_and_set(void *data)
{
	while(!kthread_should_stop()) {
		
		while(__sync_lock_test_and_set(&lock, 1))
			;
		counter += 1;
		__sync_lock_release(&lock);
		
		printk(KERN_INFO "pid[%u] %s: counter: %d\n", current->pid, __func__, counter);
		msleep(500);
	}
	
	do_exit(0);
}

static int __init test_and_set_init(void)
{
	printk(KERN_INFO "%s: Entering test and set module!\n", __func__);
	kthreads[0] = kthread_run(test_and_set, NULL, "thread 0");
	kthreads[1] = kthread_run(test_and_set, NULL, "thread 1");
	kthreads[2] = kthread_run(test_and_set, NULL, "thread 2");
	kthreads[3] = kthread_run(test_and_set, NULL, "thread 3");
	
	return 0;
}

static void __exit test_and_set_exit(void)
{
	kthread_stop(kthreads[0]);
	kthread_stop(kthreads[1]);
	kthread_stop(kthreads[2]);
	kthread_stop(kthreads[3]);
	
	printk(KERN_INFO "%s: Exiting test and set module!\n", __func__);
}

module_init(test_and_set_init);
module_exit(test_and_set_exit);
MODULE_LICENSE("GPL");
