/* date = October 6th 2024 11:56 am */

#ifndef MEMORY_H
#define MEMORY_H

typedef struct Arena {
    u8 *mem;
    u64 size;
    u64 capacity;
} Arena;

#define ArenaPushStruct(arena, type, num) ArenaPush(arena, sizeof(type) * num)

func void ArenaInit(Arena *arena, u64 size) 
{
    arena->mem = (u8*)MemoryAlloc(size);
    arena->size = 0;
    arena->capacity = size;
}

func void ArenaClear(Arena *arena) 
{
    arena->size = 0;
}

func void ArenaDestroy(Arena *arena) 
{
    MemoryFree(arena->mem);
    MemoryZero(arena, sizeof(Arena));
}

func void *ArenaPush(Arena *arena, u64 size)
{
    void *result = arena->mem + arena->size;
    arena->size += size;
    Assert(arena->size <= arena->capacity);
    
    return result;
}

func void ArenaPop(Arena *arena, u64 size)
{
    Assert(arena->size >= size);
    MemoryZero(arena->mem + arena->size, size);
    arena->size -= size;
}
#endif //MEMORY_H
