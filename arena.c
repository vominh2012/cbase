#define MEM_PAGE_SIZE (4 * 1024)
#define MEM_ALLOCATION_GRANULARITY (64 * 1024)
#define MEM_ALIGNMENT 8

#if defined(__SANITIZE_ADDRESS__)
#include "sanitizer/asan_interface.h"
#define AsanPoison __asan_poison_memory_region
#define AsanUnpoison __asan_unpoison_memory_region
#else
#define AsanPoison
#define AsanUnpoison
#endif 

MArena *ArenaNew(psize size)
{
    psize arena_size = AlignSize(sizeof(MArena), MEM_ALIGNMENT);
    psize asize = AlignSize(arena_size + size, MEM_ALLOCATION_GRANULARITY);
    MArena *arena = (MArena*)OS_MemoryAlloc(asize);
    AsanPoison(arena, asize);
    AsanUnpoison(arena, sizeof(MArena));
    
    Assert(arena); // TODO: handle failed allocate 
    
    arena->mem = (u8*)arena + arena_size;
    arena->capacity = asize - arena_size;
    SListInit(arena);
    
    return arena;
}

void ArenaFree(MArena *arena)
{
    psize arena_size = AlignSize(sizeof(MArena), MEM_ALIGNMENT);
    AsanUnpoison(arena, arena_size + arena->capacity);
    OS_MemoryFree(arena);
}

void ArenaFreeCurrentNode(MArena *sentinal)
{
    MArena *current = sentinal->next;
    SListStackPop(sentinal);
    
    ArenaFree(current);
}

void ArenaDestroy(MArena *arena) 
{
    while (SListNotEmpty(arena))
    {
        ArenaFreeCurrentNode(arena);
    }
    
    OS_MemoryZero(arena, sizeof(*arena));
    ArenaFree(arena);
}

u8 *ArenaPush(MArena *arena, psize size, bool zero)
{
    u8 *result = 0;
    MArena *current_arena = arena->next;
    
    psize asize = AlignSize(size, MEM_ALIGNMENT);
    
    if (current_arena->size + asize > current_arena->capacity)
    {
        MArena *new_arena = ArenaNew(asize);
        result = new_arena->mem;
        new_arena->size = asize;
        
        SListStackPush(arena, new_arena);
        // zero = false; // OS new alloc always zero memory
    }
    else
    {
        result = current_arena->mem + current_arena->size;
        current_arena->size += asize;
        Assert(current_arena->size <= current_arena->capacity);
    }
    
    AsanUnpoison(result, size);
    
    if (zero)
    {
        OS_MemoryZero(result, size);
    }
    
    return result;
}

void ArenaPop(MArena *arena, psize size)
{
    MArena *current_arena = arena->next;
    psize asize = AlignSize(size, MEM_ALIGNMENT);
    Assert(asize <= current_arena->size);
    current_arena->size -= asize;
    
    if (current_arena->size == 0 && current_arena != arena)
    {
        ArenaFreeCurrentNode(arena);
    }
    
}

void EndTempArena(MTempArena *temp)
{
    while (temp->arena->next != temp->current) {
        ArenaFreeCurrentNode(temp->arena);
    }
    
    temp->arena->size = temp->offset;
}
