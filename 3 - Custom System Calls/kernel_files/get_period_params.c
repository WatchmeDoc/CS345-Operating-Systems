#include <linux/kernel.h>
#include <linux/errno.h>
#include <asm/current.h>
#include <asm/uaccess.h>
#include <linux/sched.h>

asmlinkage long sys_get_period_params(struct period_params *params){
	struct task_struct *curr_task = get_current();
	printk("George Manos, csd4333, executing get_period_params!\n");
	if(!params || access_ok(VERIFY_WRITE, params, sizeof(struct period_params)) == 0){
		return EINVAL;
	}
	params->period	       = curr_task->params.period;
	params->execution_time = curr_task->params.execution_time;
	params->num_of_periods = curr_task->params.num_of_periods;
	return 0;
}
