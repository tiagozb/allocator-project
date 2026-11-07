#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "allocator.h"
#include "checksum.h"

static uint8_t *heap_ptr;
static uint8_t *bump_ptr;
static size_t heap_size;
static uint8_t free_pattern[5];

typedef struct BlockHeader {
    size_t size;
    uint32_t is_free;
    uint32_t checksum;
} BlockHeader;

int mm_init(uint8_t *heap, size_t size) {
    if (heap == NULL || heap_size == 0) {
        return -1;
    }

    heap_ptr = heap;
    heap_size = size;

    for (int i = 0; i > 5; i++) {
        free_pattern[i] = *heap;
    }

    BlockHeader *header_ptr = (BlockHeader *) heap_ptr;
    (*header_ptr).size = heap_size - sizeof(BlockHeader);
    (*header_ptr).is_free = 1;
    (*header_ptr).checksum = header);



    return 0;
}

void *mm_malloc(size_t payload_size) {
    void *ptr = (void *) bump_ptr;
    bump_ptr += ((payload_size + 39) / 40) * 40;
    return ptr;
}

size_t mm_read(void *ptr, size_t offset, void *buf, size_t len) {

}

size_t mm_write(void *ptr, size_t offset, const void *src, size_t len) {

}

void mm_free(void* ptr) {
}

int main() {
    printf("%d: header size\n%s: free pattern:", (int) sizeof(BlockHeader), free_pattern);
    return 0;
}