/* date = December 1st 2024 0:45 pm */

#ifndef ARENA_H
#define ARENA_H

typedef struct Arena {
    u8 *mem;
    psize size;
    psize capacity;
} Arena;

#define ArenaPushStruct(arena, type) (type*)ArenaPush(arena, sizeof(type))
#define ArenaPushArray(arena, type, num) (type*)ArenaPush(arena, sizeof(type) * num)

void ArenaInit(Arena *arena, psize size);
u8 *ArenaPush(Arena *arena, psize size);
void ArenaPop(Arena *arena, psize size);
void ArenaDestroy(Arena *arena);
void ArenaClear(Arena *arena);

inline void ArenaInit(Arena *arena, psize size)
{
    arena->mem = MemoryAlloc(size);
    arena->size = 0;
    arena->capacity = size;
}

inline void ArenaClear(Arena *arena) 
{
    arena->size = 0;
}

inline void ArenaDestroy(Arena *arena) 
{
    MemoryFree(arena->mem);
    MemoryZero(arena, sizeof(Arena));
}

inline u8 *ArenaPush(Arena *arena, psize size)
{
    u8*result = arena->mem + arena->size;
    arena->size += size;
    Assert(arena->size <= arena->capacity);
    
    return result;
}

inline void ArenaPop(Arena *arena, psize size)
{
    Assert(arena->size >= size);
    MemoryZero(arena->mem + arena->size, size);
    arena->size -= size;
}

#endif //ARENA_H
