#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "allocator.h"

#define ALIGNMENT 40
#define PATTERN_SIZE 5
#define ALIGN_UP(x) ((((x) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT)
#define ALIGN_DOWN(x) (((x) / 40) * 40)


static uint8_t *heap_ptr;
static size_t heap_size;

typedef struct {
    size_t payload_space;
    bool is_free;
} BlockHeader;

int mm_init(uint8_t *ptr, size_t size) {
    size_t min_heap_size = ALIGN_UP(PATTERN_SIZE) + ALIGN_UP(sizeof(BlockHeader)) + ALIGNMENT;

    if (ptr == NULL || size < min_heap_size) {
        return -1;
    }

    heap_ptr = ptr;
    heap_size = ALIGN_DOWN(size);

    BlockHeader *header_ptr = (BlockHeader *)(heap_ptr + ALIGN_UP(PATTERN_SIZE));
    header_ptr->payload_space = heap_size - ALIGN_UP(PATTERN_SIZE) - ALIGN_UP(sizeof(BlockHeader));
    header_ptr->is_free = true;

    return 0;
}

void *mm_malloc(size_t payload_size) {
    uint8_t *block_ptr = heap_ptr + ALIGN_UP(PATTERN_SIZE);
    uint8_t *heap_end = heap_ptr + heap_size;

    while (block_ptr < heap_end) {
        BlockHeader *header_ptr = (BlockHeader *)block_ptr;
        if (header_ptr->is_free && payload_size <= header_ptr->payload_space) {

            header_ptr->is_free = false;
            size_t free_block_space = header_ptr->payload_space - ALIGN_UP(payload_size);

            if (free_block_space > ALIGN_UP(sizeof(BlockHeader))) {
                header_ptr->payload_space = ALIGN_UP(payload_size);

                BlockHeader *new_header_ptr = (BlockHeader *)(block_ptr + ALIGN_UP((sizeof(BlockHeader))) + ALIGN_UP(payload_size));
                new_header_ptr->payload_space = free_block_space - ALIGN_UP(sizeof(BlockHeader));
                new_header_ptr->is_free = true;
            }
            return (void *)block_ptr + ALIGN_UP(sizeof(BlockHeader));
        }
        block_ptr += ALIGN_UP(sizeof(BlockHeader)) + header_ptr->payload_space;
    }
    return NULL;
}

int main() {
    return 0;
}