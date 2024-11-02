/* date = April 27th 2024 0:53 pm */

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "Windows.h"

typedef void* allocator_alloc_sig(void *thiz, size_t size, u32 flags);
typedef void* allocator_realloc_sig(void *thiz, void *ptr,size_t size, u32 flags);
typedef void allocator_free_sig(void *thiz, void *mem);

enum MemoryBlockStatus {
    MEMORY_BLOCK_NULL,
    MEMORY_BLOCK_FREE,
    MEMORY_BLOCK_USED
};

typedef struct MemoryBlock {
    u8 *mem;
    psize size;
    u32 status;
    
    struct MemoryBlock *next;
    struct MemoryBlock *prev;
} MemoryBlock;

typedef struct MemoryBlockFree {
    MemoryBlock block;
    struct MemoryBlockFree *next;
    struct MemoryBlockFree *prev;
} MemoryBlockFree;

typedef struct GMem {
    u8 *mem;
    psize mem_used;
    psize mem_size;
    
    psize free_blocks_count;
    MemoryBlockFree free_blocks;
    
    psize block_count;
    MemoryBlock blocks;
    
    // statictic
    psize merged_count;
    psize expand_count;
} GMem;

void GMemInit(GMem *mem, psize  mem_size);
u8* GMemAlloc(GMem *memory, psize size);
u8* GMemRealloc(GMem *memory, void* old_mem, psize size);
void GMemFree(GMem *memory, void *data);

#define struct_alloc(allocator, type) (type*)(allocator)->alloc(sizeof(type))
#define struct_free(allocator, n) if (allocator) (allocator)->free(n)

#endif //ALLOCATOR_H
