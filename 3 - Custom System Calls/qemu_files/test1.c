#include <stdio.h>
#include <unistd.h>

#include <stdlib.h>


#define __NR_set_period_params 341
#define __NR_get_period_params 342
#define PERIOD 2
#define EXEC_TIME 300
#define NO_PERIODS 3


int main(void){
	struct period_params *params = (struct period_params *) malloc(sizeof(struct period_params));
	printf("Executing test1.c\n");
	if(syscall(__NR_set_period_params, PERIOD, EXEC_TIME, NO_PERIODS)!= 0){
		printf("Error at set params!\n");
		return 1;
	}
	if(syscall(__NR_get_period_params, params) != 0){
		printf("Error at get params!\n");
		return 2;
	}
	
	printf("set and get executed without errors.\n");
	printf("Set had set  period %d, execution time %d, number of periods %d\n", PERIOD, EXEC_TIME, NO_PERIODS);
	printf("Get returned period %d, execution time %d, number of periods %d\n", params->period, params->execution_time, params->num_of_periods);
	if(params->period == PERIOD && params->execution_time == EXEC_TIME && params->num_of_periods == NO_PERIODS){
		printf("Success!\n");
	}else {
		printf("Failed.\n");
	}
	free(params);
	return 0;
}

