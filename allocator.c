#include "allocator.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define PATTERN_SIZE 5

#define ALIGNMENT 40
#define ALIGN_UP(x) ((((x) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT)
#define ALIGN_DOWN(x) (((x) / ALIGNMENT) * ALIGNMENT)
#define ALIGNMENT_NO(x) ((x) / ALIGNMENT)

static uint8_t *heap_ptr;
static size_t heap_size;

typedef struct {
    size_t block_size;
    size_t prev_block_size;
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
    header_ptr->block_size = heap_size - ALIGN_UP(PATTERN_SIZE);
    header_ptr->prev_block_size = NULL;
    header_ptr->is_free = true;

    return 0;
}

void *mm_malloc(size_t data_size) {
    uint8_t *block_ptr = heap_ptr + ALIGN_UP(PATTERN_SIZE);
    uint8_t *heap_end = heap_ptr + heap_size;

    while (block_ptr <= heap_end) {
        BlockHeader *header_ptr = (BlockHeader *)block_ptr;

        size_t data_space = header_ptr->block_size - ALIGN_UP(sizeof(BlockHeader));

        if (header_ptr->is_free && data_size <= data_space) {

            header_ptr->is_free = false;
            size_t free_block_space = data_space - ALIGN_UP(data_size);

            if (free_block_space >= ALIGN_UP(sizeof(BlockHeader)) + ALIGNMENT) {

                header_ptr->block_size -= free_block_space;

                BlockHeader *new_header_ptr = (BlockHeader *)(block_ptr + header_ptr->block_size);

                new_header_ptr->block_size = free_block_space;
                new_header_ptr->prev_block_size = header_ptr->block_size;
                new_header_ptr->is_free = true;
            }
            return block_ptr + ALIGN_UP(sizeof(BlockHeader));
        }
        block_ptr += header_ptr->block_size;
    }
    return NULL;
}

size_t mm_read(void *ptr, size_t offset, void *buf, size_t len) {
    BlockHeader *header_ptr = (BlockHeader *)((uint8_t)ptr - ALIGN_UP(sizeof(BlockHeader)));

    size_t data_space = header_ptr->block_size - ALIGN_UP(sizeof(BlockHeader));

    if (data_space < offset + len || header_ptr->is_free) {
        return -1;
    }

    uint8_t *data_ptr = (uint8_t *)ptr + offset;
    memcpy(buf, data_ptr, len);
    return len;
}

size_t mm_write(void *ptr, size_t offset, const void *src, size_t len) {
    BlockHeader *header_ptr = (BlockHeader *)((uint8_t)ptr - ALIGN_UP(sizeof(BlockHeader)));

    size_t data_space = header_ptr->block_size - ALIGN_UP(sizeof(BlockHeader));

    if (data_space < offset + len || header_ptr->is_free) {
        return -1;
    }
    uint8_t *data_ptr = (uint8_t *)ptr + offset;
    memcpy(data_ptr, src, len);
    return len;

}

void mm_free(void *ptr) {
    uint8_t *heap_end = heap_ptr + heap_size;

    uint8_t *block_ptr = (uint8_t *)ptr;
    BlockHeader *header_ptr = (BlockHeader *)((uint8_t *)ptr - ALIGN_UP(sizeof(BlockHeader)));

    if (header_ptr->is_free) {
        return -1;
    }

    uint8_t *next_block_ptr = block_ptr + header_ptr->block_size;
    while (next_block_ptr <= heap_end && ((BlockHeader *)next_block_ptr)->is_free) {
        next_block_ptr += ((BlockHeader *)next_block_ptr)->block_size;
    }

    uint8_t *prev_block_ptr = block_ptr - header_ptr->prev_block_size;
    while (prev_block_ptr >= heap_ptr + ALIGN_UP(PATTERN_SIZE) && ((BlockHeader *)prev_block_ptr)->is_free) {
        prev_block_ptr -= ((BlockHeader *)prev_block_ptr)->block_size;
    }

    uint8_t *free_block_ptr = prev_block_ptr + ((BlockHeader *)prev_block_ptr)->block_size;
    BlockHeader *free_header_ptr = (BlockHeader *)free_block_ptr;
    free_header_ptr->block_size = next_block_ptr - free_block_ptr;


    for (size_t i = ALIGN_UP(sizeof(BlockHeader)); i < free_header_ptr->block_size; i += ALIGNMENT) {
        memcpy(heap_ptr, free_block_ptr + i, ALIGNMENT);
    }

}