#include <linux/kernel.h>
#include <linux/errno.h>
#include <asm/current.h>
#include <linux/sched.h>

asmlinkage long sys_set_period_params(int period, int execution_time, int num_of_periods){
	struct task_struct *curr_struct;
	printk("George Manos, csd4333, set_period_params executed!\n");
	curr_struct = get_current();
	if(execution_time <= period*1000 &&  execution_time > 0 && num_of_periods > 0){
		curr_struct->params.period = period;
		curr_struct->params.execution_time = execution_time;
		curr_struct->params.num_of_periods = num_of_periods;
	} else
		return EINVAL;
	return 0;
}
