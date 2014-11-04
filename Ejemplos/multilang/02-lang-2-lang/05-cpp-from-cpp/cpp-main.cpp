
#include <stdio.h>
#include "cpp-sum.h"

static const int NUM=200;

int main(int, char **)
{
    int data[NUM], num, i;

    num = NUM;
    for (i=0; i < num; i++) {
        data[i] = i - 100 + 1;  /* NOTE: difference in array numbering */  
    }

    printf("sum=%d\n", sum_abs(data, num));

    return 0;
}
