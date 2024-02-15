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

    //if 0 bytes are requested
    if(size == 0) {
        return NULL;
    }

    //somehow apparently rounds up to nearest 8 using bitwise
    size_t allocated_space = ((size + sizeof(Header) +7) & ~7);

    //if number of bytes requested is greater the bytes available
    if(allocated_space >= MEMLENGTH*sizeof(double)) {
        return NULL;
    }

    //pointer to beginning of memory 
    Header* current = (Header*)memory;


    //initializing the first header
    if(current->size == 0) {
        current->size = MEMLENGTH * sizeof(double);
        current->payload = (MEMLENGTH * sizeof(double)) - sizeof(Header);
        current->free = 0;
    }


    //while the address of current pointer is less then the bounds of memory (MEMLENGTH is how long memory array spans)
    // 0 + 8(512)
    while(current < (Header*)(memory + MEMLENGTH)) {
        //if header says space is free and the amount of bytes-header space is enough to allocate
        if(current->free == 0 && current->payload >= allocated_space) {
            //extra space from, payload will always be >= space needed
            size_t extra_space = current->payload - allocated_space;


            if(extra_space >= sizeof(Header) + 1) {
                Header* next_header = (Header*)((char*)current + (sizeof(Header)+size));
                next_header->size = extra_space;
                next_header->payload = extra_space - sizeof(Header);
                next_header->free = 0;

                current->size = extra_space;
                current->payload = extra_space - sizeof(Header);
            } else {
                current->size = allocated_space;
                current->payload = allocated_space - sizeof(Header);
            }

            current->free = 1;
            return ((char*)current + sizeof(Header));
        }
        current = (Header*)((char*)current + current->size);
    }

    return NULL;
}


