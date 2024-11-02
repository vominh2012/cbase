#include <stdio.h>

typedef unsigned int u32;

u32 diff_address(void *high, void *low)
{
    u32 high_addr = (u32)high;
    u32 low_addr = (u32)low;
    u32 diff = high_addr - low_addr;
    return diff;
}

int main()
{
    int a = 0;
    int b = 0;
    void*mem = calloc(1,1024);
    void*mem1 = calloc(1,1024);
    
    printf("%p\n", &a);
    printf("%p\n", &b);
    
    printf("%p\n", mem);
    printf("%p\n", mem1);
    
    printf("stack vs stack: %d\n", diff_address(&a, &b));
    printf("stack vs heap: %d\n", diff_address(mem, &a));
    printf("heap 1 vs heap 2: %d\n", diff_address(mem1, mem));
    return 0;
}