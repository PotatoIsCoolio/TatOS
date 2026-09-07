// had AI revamp this all because you would think 8k lines of code it would work perfectly right?
// Wrong.
#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_STDIO
#define STBI_NO_SIMD
#define STBI_NO_HDR       
#define STBI_NO_LINEAR     

#define STBI_ASSERT(x) ((void)0)

#include <stddef.h>
#include <stdint.h>

int abs(int j) {
    return (j < 0) ? -j : j;
}

void* memcpy(void* dest, const void* src, size_t n) {
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;
    while (n--) *d++ = *s++;
    return dest;
}

void* memset(void* dest, int val, size_t n) {
    uint8_t* d = (uint8_t*)dest;
    while (n--) *d++ = (uint8_t)val;
    return dest;
}

// 32 MB static heap pool to prevent memory exhaustion on uncompressed high-res images
static uint8_t stb_heap[32 * 1024 * 1024];
static size_t stb_heap_offset = 0;

void* kmalloc(size_t size) {
    size_t aligned_size = (size + 15) & ~15;

    // Return NULL on out-of-memory to prevent buffer wraparound corruption
    if (stb_heap_offset + aligned_size > sizeof(stb_heap)) {
        return NULL;
    }

    void* ptr = &stb_heap[stb_heap_offset];
    stb_heap_offset += aligned_size;
    return ptr;
}

void kfree(void* ptr) {
    (void)ptr;
}

void* krealloc(void* ptr, size_t size) {
    if (!ptr) return kmalloc(size);
    
    void* new_ptr = kmalloc(size);
    if (new_ptr) {
        // Copy contents to the newly allocated block
        memcpy(new_ptr, ptr, size);
    }
    return new_ptr;
}

// Optional helper to reset the bump offset between image loads
void reset_stb_heap(void) {
    stb_heap_offset = 0;
}

#define STBI_MALLOC(sz)                    kmalloc(sz)
#define STBI_FREE(p)                       kfree(p)
#define STBI_REALLOC(p, newsz)             krealloc(p, newsz)
#define STBI_REALLOC_SIZED(p, oldsz, newsz) krealloc(p, newsz)

#include "stb_image.h"