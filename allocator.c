#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "allocator.h"
#include "checksum.h"

#define ALIGNMENT 40
#define PATTERN_SIZE 5


static uint8_t *heap_ptr;
static size_t heap_size;

typedef struct {
    size_t block_size;
    bool is_free;
    uint32_t metadata_checksum;
    uint32_t payload_checksum;
} BlockHeader;

int mm_init(uint8_t *heap, size_t size) {
    if (heap == NULL || heap_size == 0) {
        return -1;
    }

    heap_ptr = heap;
    heap_size = size;

    BlockHeader *header_ptr = (BlockHeader *) heap_ptr;
    (*header_ptr).block_size = heap_size;
    (*header_ptr).is_free = true;
    (*header_ptr).metadata_checksum = calculate_metadata_checksum(header_ptr);
    (*header_ptr).payload_checksum = calculate_payload_checksum(header_ptr, (*header_ptr).block_size);
    return 0;
}

void *mm_malloc(size_t payload_size) {
    
}

size_t mm_read(void *ptr, size_t offset, void *buf, size_t len) {

}

size_t mm_write(void *ptr, size_t offset, const void *src, size_t len) {

}

void mm_free(void* ptr) {
}