#include <stdio.h>
#include <unistd.h>


#include <stdlib.h>

#define __NR_set_period_params 341
#define __NR_get_period_params 342
#define PERIOD 2
#define EXEC_TIME 3000
#define NO_PERIODS 3


int main(void){

	printf("Executing test2.c\n");
	if(syscall(__NR_set_period_params, PERIOD, EXEC_TIME, NO_PERIODS) == 0){
		printf("Error at set params not handled!\n");
		return 1;
	}
	if(syscall(__NR_get_period_params, NULL) == 0){
		printf("Error at get params unhandled!\n");
		return 2;
	}
	
	printf("All Errors were handled successfully!\n");







	
	return 0;
}

