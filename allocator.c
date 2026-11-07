#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "allocator.h"

#define ALIGNMENT 40
#define PATTERN_SIZE 5
#define ALIGN(x) ((((x) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT)


static uint8_t *heap_ptr;
static size_t heap_size;


typedef struct {
    size_t block_size;
    bool is_free;
} BlockHeader;

int mm_init(uint8_t *heap, size_t size) {
    if (heap == NULL || heap_size < ALIGNMENT) {
        return -1;
    }

    heap_ptr = heap;
    heap_size = size;

    BlockHeader *header_ptr = (BlockHeader *) heap_ptr;
    header_ptr->block_size = heap_size;
    header_ptr->is_free = true;
    return 0;
}

void *mm_malloc(size_t payload_size) {
    BlockHeader *header_ptr = heap_ptr;

    while (header_ptr < heap_size) {
        if (header_ptr->block_size >= payload_size + sizeof(BlockHeader) && header_ptr->is_free) {
            size_t prev_block_size = header_ptr->block_size;

            header_ptr->is_free = false;
            header_ptr->block_size = ALIGN(payload_size + sizeof(BlockHeader)) - sizeof(BlockHeader);

            BlockHeader *split_header = (BlockHeader *) heap_ptr + header_ptr->block_size;
            split_header->block_size = prev_block_size - payload_size - sizeof(BlockHeader);
            split_header->is_free = true;

            return (void *) header_ptr + sizeof(BlockHeader);
        }
    }
    return NULL;
}

size_t mm_read(void *ptr, size_t offset, void *buf, size_t len) {

}

size_t mm_write(void *ptr, size_t offset, const void *src, size_t len) {

}

void mm_free(void* ptr) {
}