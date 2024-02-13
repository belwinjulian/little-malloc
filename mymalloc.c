#include "mymalloc.h"
#include <stdio.h>

#define MEMLENGTH 512
static double memory[MEMLENGTH];

typedef struct Metadata {
    size_t size;
    size_t payload;
    int free;
} Metadata;

void *mymalloc(size_t size, char *file, int line) {
    //somehow apparently rounds up to nearest 8 using bitwise
    size_t allocated_space = ((size + sizeof(Metadata) +7) & ~7);

    //pointer to beginning of memory (all metadata is in linked list)
    Metadata* current = (Metadata*)memory;

    //while the address of current pointer is less then the bounds of memory (MEMLENGTH is how long memory array spans)
    while(current < (Metadata*)(memory + MEMLENGTH)) {
        if(current -> free == 0 && current -> size >= allocated_space) {
            size_t extra_space = allocated_space - (current -> size);

            if(extra_space >= sizeof(Metadata) + 1) {
                Metadata* next_header = (Metadata*)((void*)current + allocated_space);
                next_header -> size = extra_space;
                next_header -> payload = extra_space - sizeof(Metadata);
                next_header -> free = 0;

                current -> size = allocated_space - extra_space;
                current -> payload = (allocated_space - extra_space) - sizeof(Metadata);
            } else {
                current -> size = allocated_space;
                current -> payload = allocated_space - sizeof(Metadata);
            }

            current -> free = 1;
            return ((void*)current + sizeof(Metadata));
        }

    }

    return NULL;
}
