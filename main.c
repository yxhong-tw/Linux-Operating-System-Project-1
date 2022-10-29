#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
// #include <unistd.h>


void* pt_func_1(void) {
    // for(int i = 0; i < 5; i++) {
    //     printf("This is pthread 1 and i = %d.\n", i);
    //     sleep(1);
    // }

    // Write thread 1 code here.

    pthread_exit(NULL);
}


void* pt_func_2(void) {
    // for(int i = 0; i < 5; i++) {
    //     printf("This is pthread 2 and i = %d.\n", i);
    //     sleep(1);
    // }

    // Write thread 2 code here.

    pthread_exit(NULL);
}


int main(void) {
    pthread_t pt_1 = 0;
    pthread_t pt_2 = 0;

    pthread_create(&pt_1, NULL, (void*)pt_func_1, "pt_func_1");
    pthread_create(&pt_2, NULL, (void*)pt_func_2, "pt_func_2");

    // for(int i = 0; i < 5; i++) {
    //     printf("This is main thread and i = %d\n", i);
    //     sleep(1);
    // }

    pthread_join(pt_1, NULL);
    pthread_join(pt_2, NULL);

    return 0;
}