#include <stddef.h>
#include <stdint.h>
#include "allocator.h"

static uint8_t *heap_start;
static size_t heap_size;

typedef struct BlockHeader {
    size_t size;
    int is_free;
} BlockHeader;

int mm_init(uint8_t *heap, size_t size) {
    if (heap == NULL || heap_size == 0) {
        return -1;
    }
    heap_start = heap;
    heap_size = size;
    return 0;
}

void *mm_malloc(size_t size) {
    void *ptr = (void *) heap_start;
    heap_start += size;
    return ptr;
}