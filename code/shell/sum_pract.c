#include <stdio.h>

int main(void)
{
    int wSum;
    
    for(int i = 1; i <= 100; i++)
    {
        wSum += i; 
    }
    printf("%d", wSum);    
    return 0;
}
