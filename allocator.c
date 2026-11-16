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
    size_t data_space;
    uint8_t *prev_block_ptr;
    bool is_free;
} BlockHeader;

bool is_valid_pointer() {

}

int mm_init(uint8_t *ptr, size_t size) {
    size_t min_heap_size = ALIGN_UP(PATTERN_SIZE) + ALIGN_UP(sizeof(BlockHeader)) + ALIGNMENT;

    if (ptr == NULL || size < min_heap_size) {
        return -1;
    }

    heap_ptr = ptr;
    heap_size = ALIGN_DOWN(size);

    BlockHeader *header_ptr = (BlockHeader *)(heap_ptr + ALIGN_UP(PATTERN_SIZE));
    header_ptr->data_space = heap_size - ALIGN_UP(PATTERN_SIZE) - ALIGN_UP(sizeof(BlockHeader));
    header_ptr->prev_block_ptr = NULL;
    header_ptr->is_free = true;

    return 0;
}

void *mm_malloc(size_t data_size) {
    uint8_t *block_ptr = heap_ptr + ALIGN_UP(PATTERN_SIZE);
    uint8_t *heap_end = heap_ptr + heap_size;

    while (block_ptr < heap_end) {
        BlockHeader *header_ptr = (BlockHeader *)block_ptr;

        if (header_ptr->is_free && data_size <= header_ptr->data_space) {

            header_ptr->is_free = false;
            size_t free_block_space = header_ptr->data_space - ALIGN_UP(data_size);

            if (free_block_space >= ALIGN_UP(sizeof(BlockHeader)) + ALIGNMENT) {

                header_ptr->data_space = ALIGN_UP(data_size);

                BlockHeader *new_header_ptr = (BlockHeader *)(block_ptr + ALIGN_UP((sizeof(BlockHeader))) + ALIGN_UP(data_size));
                new_header_ptr->data_space = free_block_space - ALIGN_UP(sizeof(BlockHeader));
                new_header_ptr->prev_block_ptr = block_ptr;
                new_header_ptr->is_free = true;
            }
            return block_ptr + ALIGN_UP(sizeof(BlockHeader));
        }
        block_ptr += ALIGN_UP(sizeof(BlockHeader)) + header_ptr->data_space;
    }
    return NULL;
}

size_t mm_read(void *ptr, size_t offset, void *buf, size_t len) {
    BlockHeader *header_ptr = (BlockHeader *)(((uint8_t)ptr) - ALIGN_UP(sizeof(BlockHeader)));

    if (header_ptr->data_space < offset + len && header_ptr->is_free) {
        return -1;
    }
    uint8_t *data_ptr = ((uint8_t *)ptr) + len;
    memcpy(buf, data_ptr, len);
    return len;
}

size_t mm_write(void *ptr, size_t offset, const void *src, size_t len) {
    BlockHeader *header_ptr = (BlockHeader *)(((uint8_t)ptr) - ALIGN_UP(sizeof(BlockHeader)));

    if (header_ptr->data_space < offset + len && header_ptr->is_free) {
        return -1;
    }
    uint8_t *data_ptr = ((uint8_t *)ptr) + len;
    memcpy(data_ptr, src, len);
    return len;

}

void mm_free(void *ptr) {
    
}