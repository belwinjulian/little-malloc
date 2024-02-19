#include <stdio.h>
#include "mymalloc.h"


#define MEMLENGTH 512
static double memory[MEMLENGTH];

typedef struct Header {
    short size;
    short payload;
    short free;
} Header;
//since both variable types are integers, both are initialized to 0

void *mymalloc(size_t size, char *file, int line) {

    //if 0 bytes are requested
    if(size == 0) {
         fprintf(stderr, "Memory Allocation Failed: Can't allocate 0 bytes (%s:%d).\n", file, line);
        return NULL;
    }

    //somehow apparently rounds up to nearest 8 using bitwise
    size_t allocated_space = ((size + sizeof(Header) +7) & ~7);

    //if number of bytes requested is greater the bytes available
    if(allocated_space >= MEMLENGTH*sizeof(double)) {
         fprintf(stderr, "Memory Allocation Failed: Requested more than available space (%s:%d).\n", file, line);
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
           
        //if there is enough extra space for header and atleast 1 byte, split the chunk (two headers)
       
            if(extra_space >= sizeof(Header) + 1) { 
                //address + size of current header + size of payload will take u to the start of next header (initialize header as well)
                Header* next_header = (Header*)((char*)current + allocated_space);
                //set all values of the next header
                next_header->size = extra_space;
                next_header->payload = extra_space - sizeof(Header);
                next_header->free = 0;
            } 

                current->size = allocated_space;
                current->payload = allocated_space - sizeof(Header);
            
            //set the current header to being used
            current->free = 1;
            //return address of payload (address of header + bytes header takes up)
            return ((char*)current + sizeof(Header));
        }
        //traverse to next header
        current = (Header*)((char*)current + current->size);
    }

     fprintf(stderr, "Memory Allocation Failed: Not enough memory available (%s:%d).\n", file, line);
    return NULL;
}


void myfree(void *ptr, char *file, int line)
{
    if(!ptr)//no ptr error case
    {
        printf("Error: no pointer given");
        return;

    }


    // Error check: ensure ptr is within the range of our managed memory
    if (ptr < (void*)memory || ptr >= (void*)(memory + MEMLENGTH)) {
        fprintf(stderr, "Error: Attempted to free an address not obtained from malloc (%s:%d).\n", file, line);
        return;
    }


    // Get the header associated with this pointer
    Header* header = (Header*)((char*)ptr - sizeof(Header));


// Error check: ensure ptr is at the start of a chunk
    Header* temp = (Header*)memory;
    int sum = 0;

    while(temp < (Header*)(memory + MEMLENGTH)) {
        temp = (Header*)((char*)temp + temp->size);
        int sum = sum + temp->size;

        if(ptr == (void*)((char*)temp+sizeof(Header))) {
            break;
        }
        if(sum >= sizeof(double)*MEMLENGTH) {
        fprintf(stderr, "Error: Attempted to free an address not at the start of a chunk (%s:%d).\n", file, line);
        return;
        }
    }

    


    if(header->free == 0)//make sure block is not already free
    {
        fprintf(stderr, "myfree: double free attempt (%s:%d)\n", file, line);
        return;
    }

     

    // Mark the block as free
    header->free = 0;

    // Merge with next block if it is free
    Header* next = (Header*)((char*)header + header->size);
    if (next < (Header*)(memory + MEMLENGTH) && next->free == 0) {
        header->size += next->size+6;
        header->payload += next->size+6;
    }

    // Merge with previous block if it is free
    Header* prev = NULL;
    Header* current = (Header*)memory;
    while(current < (Header*)(memory + MEMLENGTH) && current < header) {
        if (current->free == 0) {
            prev = current;
        }
        current = (Header*)((char*)current + current->size);
    }
    if (prev && prev->free == 0) {
        prev->size += header->size+6;
        prev->payload += header->size+6;
    } 
} 
