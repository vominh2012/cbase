/* date = December 1st 2024 0:45 pm */

#ifndef ARENA_H
#define ARENA_H

typedef struct MArena {
    u8 *mem;
    psize size;
    psize capacity;
    
    MArena *next;
} MArena;

typedef struct MTempArena {
    MArena *arena;
    psize offset;
} MTempArena;

inline MTempArena BeginTempArena(MArena *arena)
{
    return {arena, arena->size};
}

inline void EndTempArena(MTempArena *temp)
{
    temp->arena->size = temp->offset;
}

#define ArenaPushStruct(arena, type) (type*)ArenaPush(arena, sizeof(type))
#define ArenaPushArray(arena, type, num) (type*)ArenaPush(arena, sizeof(type) * num)

void ArenaInit(MArena *arena, psize size);
u8 *ArenaPush(MArena *arena, psize size, bool zero = true);
void ArenaPop(MArena *arena, psize size);
void ArenaDestroy(MArena *arena);
void ArenaClear(MArena *arena);

#endif //ARENA_H
