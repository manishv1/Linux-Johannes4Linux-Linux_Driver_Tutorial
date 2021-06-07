#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>

/* Global variables for the threads */
static struct task_struct *kthread_1;
static struct task_struct *kthread_2;
static int t1 = 1, t2 = 2;

/**
 * @brief Function which will be executed by the threads
 *
 * @param thread_nr	Pointer to number of the thread
 */
int thread_function(void * thread_nr) {
	unsigned int i = 0;
	int t_nr = *(int *) thread_nr;

	/* Working loop */
	while(!kthread_should_stop()){
			printk("kthread - Thread %d is executed! Counter val: %d\n", t_nr, i++);
			msleep(t_nr * 1000);
	}

	printk("kthread - Thread %d finished execution!\n", t_nr);
	return 0;
}

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A simple example for threads in a LKM");

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
	printk("kthread - Init threads\n");

	kthread_1 = kthread_create(thread_function, &t1, "kthread_1");
	if(kthread_1 != NULL){
		/* Let's start the thread */
		wake_up_process(kthread_1);
		printk("kthread - Thread 1 was created and is running now!\n");
	}
	else {
		printk("kthread - Thread 1 could not be created!\n");
		return -1;
	}
	kthread_2 = kthread_run(thread_function, &t2, "kthread_2");
	if(kthread_2 != NULL)
		printk("kthread - Thread 2 was created and is running now!\n");
	else {
		printk("kthread - Thread 2 could not be created!\n");
		kthread_stop(kthread_1);
		return -1;
	}
	printk("kthread - Both threads are running now!\n");

	return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	printk("kthread - Stop both threads\n");
	kthread_stop(kthread_1);
	kthread_stop(kthread_2);
}

module_init(ModuleInit);
module_exit(ModuleExit);


