#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/ktime.h>
#include <linux/types.h>
#include <linux/compiler.h>

asmlinkage void sys_my_add_get_time(struct timespec __user *ptr) {
	struct timespec curTime;
	getnstimeofday(&curTime);
	copy_to_user(ptr, &curTime, sizeof(struct timespec));
	return;
}
