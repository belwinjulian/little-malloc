#include "mymalloc.h"
#include <stdio.h>

#define MEMLENGTH 512 // Memory pool size

typedef struct header {
    size_t size;
    int is_free;
    struct header *next;
} header_t;

static double memory[MEMLENGTH];
static header_t *head = NULL;

header_t *get_free_block(size_t size) {
    header_t *curr = head;
    while(curr) {
        if (curr->is_free && curr->size >= size)
            return curr;
        curr = curr->next;
    }
    return NULL;
}

void *mymalloc(size_t size, char *file, int line) {
    size_t total_size;
    void *block;
    header_t *header;
   
    if (!size) return NULL; //returns null since nothing was requested
    
    header = get_free_block(size);

    if (header) {
        header->is_free = 0;
        return (void*)(header + 1);
    }

    total_size = sizeof(header_t) + size;
    block = memory;

     if (!block) {
        printf("Error: failed to allocate memory in file %s at line %d\n", file, line);
        return NULL;
     }//prints Error and returns null 


    




}