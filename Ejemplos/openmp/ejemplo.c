#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int sum = 0;
int tid;
int array[10];

int main(){
	
	int i = 0;
	for(i = 0; i<=10; i++){
		array[i] = i;
	}

	#pragma omp parallel
	{
		tid = omp_get_thread_num();
	}

	#pragma omp parallel for reduction(+:sum)
	for (i=0; i <= 10; i++){
	    sum = sum + array[i];
	    printf("PID sumarizando es %d\n", tid);
	}

	printf("%d\n", sum);
}
