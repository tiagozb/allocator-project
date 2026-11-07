#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "allocator.h"

#define ALIGNMENT 40
#define PATTERN_SIZE 5
#define ALIGN_UP(x) ((((x) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT)


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

    BlockHeader *header_ptr = (BlockHeader *)heap_ptr;
    header_ptr->block_size = heap_size;
    header_ptr->is_free = true;
    return 0;
}

void *mm_malloc(size_t payload_size) {
    uint8_t *block_ptr = heap_ptr;
    uint8_t *heap_end = heap_ptr + heap_size;

    while (block_ptr < heap_end) {
        BlockHeader *header_ptr = (BlockHeader *) block_ptr;

        if (header_ptr->block_size >= payload_size + sizeof(BlockHeader) && header_ptr->is_free) {
            size_t prev_block_size = header_ptr->block_size;

            header_ptr->is_free = false;
            header_ptr->block_size = ALIGN_UP(payload_size + 2 * sizeof(BlockHeader)) - sizeof(BlockHeader);

            BlockHeader *split_block_header_ptr = (BlockHeader *)(block_ptr + header_ptr->block_size);
            split_block_header_ptr->block_size = prev_block_size - payload_size - sizeof(BlockHeader);
            split_block_header_ptr->is_free = true;

            return (void *) (header_ptr + sizeof(BlockHeader));
        }
        block_ptr += header_ptr->block_size;
    }
    return NULL;
}