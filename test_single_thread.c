#include <stdio.h>
#include <unistd.h>

#define ARR_SIZE 4

int var3 = 1;
int var4 = 0;

// 385

void hello_kernel() {
    syscall(385);
}

// 386

unsigned long virt_to_phy(unsigned long address) {
    return syscall(386, address);
}

int main() {
    int var1 = 0;
    long *var2 = malloc(sizeof(long));
    unsigned long address[ARR_SIZE];
    unsigned long ret;

    address[0] = &var1; //stack
    address[1] = var2;  //heap
    address[2] = &var3; //global 1
    address[3] = &var4; //bss

    hello_kernel();

    for(int i = 0; i < ARR_SIZE; i++) {
        ret = virt_to_phy(address[i]);
        printf("%p\n", address[i]);
        printf("%p\n", ret);
    }
    return 0;
}

