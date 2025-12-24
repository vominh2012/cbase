/* date = December 1st 2024 0:45 pm */

#ifndef ARENA_H
#define ARENA_H

typedef struct MArena {
    u8 *mem;
    psize size;
    psize commit_size;
    psize capacity;
    
    MArena *next;
} MArena;

typedef struct MTempArena {
    MArena *arena;
    MArena *current;
    psize offset;
} MTempArena;

#define ArenaPushStruct(arena, type) (type*)ArenaPush(arena, sizeof(type))
#define ArenaPushArray(arena, type, num) (type*)ArenaPush(arena, sizeof(type) * num)

MArena* ArenaNew(psize size);
u8 *ArenaPush(MArena *arena, psize size, bool zero = true);
void ArenaPop(MArena *arena, psize size);
void ArenaDestroy(MArena *arena);
void ArenaClear(MArena *arena);

inline MTempArena BeginTempArena(MArena *arena)
{
    return {arena, arena->next, arena->next->size};
}

void EndTempArena(MTempArena *temp);


#endif //ARENA_H
