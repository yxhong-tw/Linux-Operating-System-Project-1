#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


typedef struct {
    char *segment_name;
    void *address;
} data_format;


int for_data_segment = 48763;
int for_bss_segment = 0;
int *for_heap_segment;
void *for_libraries_segment = &printf;
void for_text_segment() {}


void* child_thread(void) {
    int for_stack_segment = 357;

    data_format items[] = {
        {"text", &for_text_segment}
        , {"data", &for_data_segment}
        , {"bss", &for_bss_segment}
        , {"heap", for_heap_segment}
        , {"libraries", for_libraries_segment}
        , {"stack", &for_stack_segment}
    };
	int i = 0;	

    for(; i < (sizeof(items)/sizeof(items[0])); i++) {
        
		unsigned long virtual = items[i].address;
		unsigned long physical = syscall(351, virtual);

		printf("Segment Name: %s, Virutal Address: %p, Physicall Address: %p\n"
            , items[i].segment_name
            , items[i].address
			, (void *)physical
        );
    }

    pthread_exit(NULL);
}


int main(void) {
    for_heap_segment = malloc(10);
    for_libraries_segment = &printf;

    pthread_t pt_1 = 0;
    pthread_t pt_2 = 0;

    pthread_create(&pt_1, NULL, (void*)child_thread, "pt_1");
    pthread_create(&pt_2, NULL, (void*)child_thread, "pt_2");

    pthread_join(pt_1, NULL);
    pthread_join(pt_2, NULL);

    return 0;
}
