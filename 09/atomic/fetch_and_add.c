#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>

#define KTHREAD_COUNT 4

struct task_struct* kthreads[KTHREAD_COUNT];

int counter = 0;
int lock = 0;

static int fetch_and_add(void *data)
{
	while(!kthread_should_stop()) {
		
		__sync_fetch_and_add(&counter, 1);
		
		printk(KERN_INFO "pid[%u] %s: counter %d\n", current->pid, __func__, counter);
		msleep(500);
	}
	
	do_exit(0);
}

static int __init fetch_and_add_init(void)
{
	printk(KERN_INFO "%s: Entering fetch and add module!\n", __func__);
	kthreads[0] = kthread_run(fetch_and_add, NULL, "thread 0");
	kthreads[1] = kthread_run(fetch_and_add, NULL, "thread 1");
	kthreads[2] = kthread_run(fetch_and_add, NULL, "thread 2");
	kthreads[3] = kthread_run(fetch_and_add, NULL, "thread 3");
	
	return 0;
}

static void __exit fetch_and_add_exit(void)
{
	kthread_stop(kthreads[0]);
	kthread_stop(kthreads[1]);
	kthread_stop(kthreads[2]);
	kthread_stop(kthreads[3]);
	
	printk(KERN_INFO "%s: Exiting fetch and add module!\n", __func__);
}

module_init(fetch_and_add_init);
module_exit(fetch_and_add_exit);
MODULE_LICENSE("GPL");
