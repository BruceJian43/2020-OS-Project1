#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/ktime.h>
#include <linux/types.h>
#include <linux/compiler.h>

asmlinkage void sys_my_add_printk(pid_t pid, const struct timespec __user *ptr1, const struct timespec __user *ptr2) {
	struct timespec start, end;
	copy_from_user(&start, ptr1, sizeof(struct timespec));
	copy_from_user(&end, ptr2, sizeof(struct timespec));
	printk("[Project1] %d %lu.%09lu %lu.%09lu\n", pid, start.tv_sec, start.tv_nsec, end.tv_sec, end.tv_nsec);
	return;
}
