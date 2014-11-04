
#include <stdio.h>

#define NUM 200

int main(int argc, char **argv)
{
    int data[NUM], num, i;

    int result = 0;    

    num = NUM;
    for (i=0; i < num; i++) {
        data[i] = i - 100 + 1;  /* NOTE: difference in array numbering */  
    }

    sum_abs_(data, &num, &result);

    printf("sum=%d\n", result);
    return 0;
}
