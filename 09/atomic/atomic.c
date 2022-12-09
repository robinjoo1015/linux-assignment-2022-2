#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>

#define KTHREAD_COUNT 4

struct task_struct* kthreads[KTHREAD_COUNT]

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

static int test_and_set(void *data)
{
	while(!kthread_should_stop()) {
		
		while(__sync_lock_test_and_set(&lock, 1))
			;
		counter += 1;
		__sync_lock_release(&lock);
		
		prinkt(KERN_INFO "pid[%u] %s: counter: %d\n", current->pid, __func__, counter);
		msleep(500);
	}
	
	do_exit(0);
}

static int compare_and_swap(void *data)
{	
	while(!kthread_should_stop()) {
		
		while(__sync_val_compare_and_swap(&lock, 0, 1))
			;
		counter += 1;
		lock = 0;
		
		prinkt(KERN_INFO "pid[%u] %s: counter: %d\n", current->pid, __func__, counter);
		msleep(500);
	}
	
	do_exit(0);
}

static int __init atomic_operation_init(void)
{
	printk(KERN_INFO "%s: Entering atomic operations module!", __func__);
	kthreads[0] = kthread_run(fetch_and_add, NULL, "fetch and add");
	kthreads[1] = kthread_run(test_and_set, NULL, "test and set");
	kthreads[2] = kthread_run(compare_and_swap, NULL, "compare and swap");
	kthreads[3] = kthread_run(fetch_and_add, NULL, "fetch and add");
	
	return 0;
}

static void __exit atomic_operation_exit(void)
{
	printk(KERN_INFO "%s: Exiting atomic operations module!", __func__);
	kthread_stop(kthreads[0]);
	kthread_stop(kthreads[1]);
	kthread_stop(kthreads[2]);
	kthread_stop(kthreads[3]);
}

module_init(atomic_operation_init);
module_exit(atomic_operation_exit);
MODULE_LICENSE("GPL");
