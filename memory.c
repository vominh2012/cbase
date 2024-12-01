
void MemoryPoolInit(MemoryPool *mem_pool, psize size, psize count)
{
    psize chunk_size = BMin(size, sizeof(DListNode));
    psize total_size = chunk_size * count;
    
    mem_pool->mem = (u8*)MemoryAlloc(total_size);
    mem_pool->pos = mem_pool->mem;
    mem_pool->last = mem_pool->mem + total_size - chunk_size;
    mem_pool->chunk_size = chunk_size;
    mem_pool->capacity = total_size;
    
    DListInit(&mem_pool->free_list);
}

void *MemoryPoolGet(MemoryPool *mem_pool)
{
    if (mem_pool->pos <= mem_pool->last)
    {
        u8* mem = mem_pool->pos;
        mem_pool->pos += mem_pool->chunk_size;
        return mem;
    }
    else if (DListNotEmpty(&mem_pool->free_list))
    {
        DListNode *free_node = DListBegin(&mem_pool->free_list);
        DListRemove(free_node);
        return free_node;
    }
    
    return 0;
}

void MemoryPoolRelease(MemoryPool *mem_pool, void *mem)
{
    u8 *mem_u8 = (u8*)mem;
    Assert((mem_u8 >= mem_pool->mem) && (mem_u8 <= mem_pool->last)); // check valid range
    DListNode *free_node = (DListNode *)mem;
    DListInsertFront(&mem_pool->free_list, free_node);
}

void MemoryPoolDestroy(MemoryPool *mem_pool)
{
    MemoryFree(mem_pool->mem);
}