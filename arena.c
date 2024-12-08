#define MEM_PAGE_SIZE (4 * 1024)
#define MEM_ALLOCATION_GRANULARITY (64 * 1024)
#define MEM_ALIGNMENT sizeof(void*)

void ArenaInit(MArena *arena, psize size)
{
    psize asize = AlignSize(size, MEM_ALLOCATION_GRANULARITY);
    arena->mem = (u8*)OS_MemoryAlloc(asize);
    arena->size = 0;
    arena->capacity = asize;
    arena->next = arena;
}

void ArenaDestroy(MArena *arena) 
{
    while (SListNotEmpty(arena))
    {
        MArena *current = arena->next;
        SListPop(arena);
        OS_MemoryFree(current);
    }
    
    OS_MemoryFree(arena->mem);
    OS_MemoryZero(arena, sizeof(*arena));
}

u8 *ArenaPush(MArena *arena, psize size, bool zero)
{
    MArena *current_arena = arena->next;
    
    psize asize = AlignSize(size, MEM_ALIGNMENT);
    u8*result = 0;
    if (current_arena->size + asize > current_arena->capacity)
    {
        psize arena_size = AlignSize(sizeof(*arena), MEM_ALIGNMENT);
        
        psize mem_size = AlignSize(asize + arena_size, MEM_ALLOCATION_GRANULARITY);
        MArena *new_arena = (MArena*)OS_MemoryAlloc(mem_size);
        new_arena->mem = (u8*)new_arena  + arena_size;
        new_arena->capacity = mem_size - arena_size;
        new_arena->size = asize;
        SListInsert(arena, new_arena);
        
        result = new_arena->mem;
        zero = false; // OS new alloc always zero memory
    }
    else
    {
        result = current_arena->mem + current_arena->size;
        current_arena->size += asize;
        Assert(current_arena->size <= current_arena->capacity);
    }
    
    if (zero)
    {
        OS_MemoryZero(result, asize);
    }
    
    return result;
}

void ArenaPop(MArena *arena, psize size)
{
    MArena *current_arena = arena->next;
    psize asize = AlignSize(size, MEM_ALIGNMENT);
    if (current_arena->size == 0)
    {
        SListPop(arena);
        OS_MemoryFree(current_arena);
        
        current_arena = arena->next;
        
        Assert(current_arena->size >= asize);
        current_arena->size -= asize;
    }
}