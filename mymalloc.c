#include <stdio.h>
#include "mymalloc.h"


#define MEMLENGTH 512
static double memory[MEMLENGTH];

typedef struct Header {
    size_t size;
    size_t payload;
    int free;
} Header;
//since both variable types are integers, both are initialized to 0

void *mymalloc(size_t size, char *file, int line) {

    if(size == 0) {
        return NULL;
    }
    //somehow apparently rounds up to nearest 8 using bitwise
    size_t allocated_space = ((size + sizeof(Header) +7) & ~7);

    if(allocated_space >= MEMLENGTH*sizeof(memory)) {
        return NULL;
    }

    //pointer to beginning of memory (all metadata is in linked list)
    Header* current = (Header*)memory;

    if(current->size == 0) {
        current->size = MEMLENGTH * sizeof(memory);
        current->payload = (MEMLENGTH * sizeof(memory)) - sizeof(Header);
        current->free = 0;
    }
    //while the address of current pointer is less then the bounds of memory (MEMLENGTH is how long memory array spans)
    // 0 + 8(512)
    while(current < (Header*)(memory + MEMLENGTH)) {
        if(current->free == 0 && current->payload >= allocated_space) {
            size_t extra_space = current->payload - allocated_space;

            if(extra_space >= sizeof(Header) + 1) {
                Header* next_header = (Header*)((char*)current + (sizeof(Header)+size));
                next_header->size = extra_space;
                next_header->payload = extra_space - sizeof(Header);
                next_header->free = 0;

                current->size = allocated_space - extra_space;
                current->payload = (allocated_space - extra_space) - sizeof(Header);
            } else {
                current->size = allocated_space;
                current->payload = allocated_space - sizeof(Header);
            }

            current->free = 1;
            return ((char*)current + sizeof(Header));
        }
        current = (Header*)((char*)current + current -> size);
    }

    return NULL;
}


