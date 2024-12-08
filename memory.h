/* date = October 6th 2024 11:56 am */

#ifndef MEMORY_H
#define MEMORY_H

typedef struct MemoryPool {
    u8 *mem;
    u8 *pos;
    u8 *last;
    psize chunk_size;
    psize capacity;
    
    DListNode free_list;
} MemoryPool;

void MemoryPoolInit(MemoryPool *mem_pool, psize size, psize count);
void *MemoryPoolGet(MemoryPool *mem_pool);
void MemoryPoolRelease(MemoryPool *mem_pool, void *mem);
void MemoryPoolDestroy(MemoryPool *mem_pool);

#endif //MEMORY_H
