#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/list.h>
#include <linux/time.h>

#define BILLION 1000000000UL
#define MILLION 10000
#define THREAD_RANGE 2500
#define KTHREAD_COUNT 4

int counter;
struct mutex counter_lock;
struct timespec spclock[2];
struct task_struct* kthreads[KTHREAD_COUNT];
unsigned long long delay_list[3];

struct my_node {
	struct list_head list;
	int data;
};
struct list_head my_list;

unsigned long long calclock(struct timespec *spclock);

void add_to_list(int thread_id, int range_bound[])
{
	int num = range_bound[0];
	printk(KERN_INFO "thread #%d range: %d ~ %d\n", thread_id, range_bound[0], range_bound[1]);
	while(num<=range_bound[1]) {
		struct my_node *new = kmalloc(sizeof(struct my_node), GFP_KERNEL);
		mutex_lock(&counter_lock);
		new->data = num;
		list_add(&new->list, &my_list);
		num++;
		counter++;
		if(counter==MILLION) {
			getnstimeofday(&spclock[1]);
			delay_list[0] = calclock(spclock);
			getnstimeofday(&spclock[0]);
		}
		mutex_unlock(&counter_lock);
	}
	while(counter<MILLION) {
		msleep(1);
		continue;
	}
	return;
}

int search_list(int thread_id, int range_bound[])
{
	struct my_node *cur;
	int num = range_bound[0];
	while(num<=range_bound[1]) {
		mutex_lock(&counter_lock);
		cur = list_first_entry(&my_list, struct my_node, list);
		while(cur->data != num) {
			cur = list_next_entry(cur, list);
		}
		num++;
		counter++;
		if(counter==2*MILLION) {
			getnstimeofday(&spclock[1]);
			delay_list[1] = calclock(spclock);
			getnstimeofday(&spclock[0]);
		}
		mutex_unlock(&counter_lock);
	}
	printk(KERN_INFO "thread #%d searched range: %d ~ %d\n", thread_id, range_bound[0], range_bound[1]);
	while(counter<2*MILLION) {
		msleep(1);
		continue;
	}
	return 0;
}

int delete_from_list(int thread_id, int range_bound[])
{
	struct my_node *cur, *tmp;
	int num = range_bound[0];
	while(num<=range_bound[1]) {
		mutex_lock(&counter_lock);
		cur = list_first_entry(&my_list, struct my_node, list);
		while(cur->data != num) {
			cur = list_next_entry(cur, list);
		}
		tmp = cur;
		cur = list_next_entry(tmp, list);
		list_del(&tmp->list);
		kfree(tmp);
		num++;
		counter++;
		if(counter==3*MILLION) {
			getnstimeofday(&spclock[1]);
			delay_list[2] = calclock(spclock);
		}
		mutex_unlock(&counter_lock);
	}
	printk(KERN_INFO "thread #%d deleted range: %d ~ %d\n", thread_id, range_bound[0], range_bound[1]);
	return 0;
}

static int linked_list_function(void *data) {
	int thread;
	int range_bound[2];
	thread = *((int *) data);
	range_bound[0] = (thread-1)*THREAD_RANGE;
	range_bound[1] = thread*THREAD_RANGE-1; 
	add_to_list(thread, range_bound);
	search_list(thread, range_bound);
	delete_from_list(thread, range_bound);
	do_exit(0);
}

static int __init mutex_module_init(void) {
	static const int thread[4] = {1, 2, 3, 4};
	printk(KERN_INFO "%s, Entering Mutex Module!\n", __func__);
	counter = 0;
	INIT_LIST_HEAD(&my_list);
	mutex_init(&counter_lock);
	getnstimeofday(&spclock[0]);
	kthreads[0] = kthread_run(linked_list_function, (void*)(thread+0), "thread1");
    	kthreads[1] = kthread_run(linked_list_function, (void*)(thread+1), "thread2");
    	kthreads[2] = kthread_run(linked_list_function, (void*)(thread+2), "thread3");
    	kthreads[3] = kthread_run(linked_list_function, (void*)(thread+3), "thread4");
	return 0;
}

static void __exit mutex_module_exit(void) {
	printk(KERN_INFO "%s: Mutex linked list insert time: %lld ns, count: %d\n", __func__, delay_list[0], MILLION);
	printk(KERN_INFO "%s: Mutex linked list search time: %lld ns, count: %d\n", __func__, delay_list[1], MILLION);
	printk(KERN_INFO "%s: Mutex linked list delete time: %lld ns, count: %d\n", __func__, delay_list[2], MILLION);
	printk("thread #1 stopped!\n");
	printk("thread #2 stopped!\n");
	printk("thread #3 stopped!\n");
	printk("thread #4 stopped!\n");
	printk(KERN_INFO "%s, Exiting Mutex Module\n", __func__);
}

module_init(mutex_module_init);
module_exit(mutex_module_exit);
MODULE_LICENSE("GPL");

unsigned long long calclock(struct timespec *spclock)
{
	unsigned long long temp, temp_n, timedelay = 0;
	if(spclock[1].tv_nsec >= spclock[0].tv_nsec) {
		temp = spclock[1].tv_sec - spclock[0].tv_sec;
		temp_n = spclock[1].tv_nsec - spclock[0].tv_nsec;
		timedelay = BILLION * temp + temp_n;
	} else {
		temp = spclock[1].tv_sec - spclock[0].tv_sec + 1;
		temp_n = BILLION + spclock[1].tv_nsec - spclock[0].tv_nsec;
		timedelay = BILLION * temp + temp_n;
	}
	return timedelay;
}
