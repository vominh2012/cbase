/* date = October 6th 2024 11:56 am */

#ifndef MEMORY_H
#define MEMORY_H

typedef struct Arena {
    u8 *mem;
    u64 size;
    u64 capacity;
} Arena;

#define ArenaPushStruct(arena, type, num) ArenaPush(arena, sizeof(type) * num)

void ArenaInit(Arena *arena, psize size);
u8 *ArenaPush(Arena *arena, psize size);
void ArenaPop(Arena *arena, psize size);
void ArenaDestroy(Arena *arena);
void ArenaClear(Arena *arena);

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
