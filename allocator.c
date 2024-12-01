void GMemInit(GMem *mem, psize mem_size) {
    DListInit(&mem->free_blocks);
    DListInit(&mem->blocks);
    
    mem->mem = (u8*)VirtualAlloc(0, mem_size,
                                 MEM_COMMIT | MEM_RESERVE,
                                 PAGE_READWRITE);
    
    mem->mem_size = mem_size;
    
    Assert(mem != 0);
}

void mem_copy(void *src, void *dest, u64 size) {
    u64 copy_bytes = size; 
    u8 *u8_src = (u8*)src;
    u8 *u8_dest = (u8*)dest;
    while (copy_bytes--) {
        *(u8_dest++) = *(u8_src++);
    }
}

b8 split_if_posible(struct MemoryBlockFree *n, psize size) 
{
    b8 splited = FALSE;
    
    psize remaining_bytes = n->block.size - size;
    if (remaining_bytes >= sizeof(MemoryBlockFree)) {
        
        n->block.size = size;
        
        MemoryBlockFree *free_block = (MemoryBlockFree *)(n->block.mem + size);
        
        MemoryBlock *block = &free_block->block;
        block->mem = (u8*)block + sizeof(MemoryBlock);
        block->size = remaining_bytes - sizeof(MemoryBlock);
        block->status = MEMORY_BLOCK_FREE;
        
        DListInsert(n, free_block);
        DListInsert(&n->block, block);
        
        splited = TRUE;
    }
    
    return splited;
}

void mem_fill_zero(u8* u8_data, psize num_bytes)
{
    while (num_bytes-- > 0) {
        *(u8_data++) = 0;
    }
}

b8 merge_block_if_posible(GMem *memory, MemoryBlock *first, MemoryBlock *second) {
    b8 merged = FALSE;
    
    if (first->status == MEMORY_BLOCK_FREE && second->status == MEMORY_BLOCK_FREE) {
        // is contiguous
        if ((u8*)(first->mem) + first->size == (u8*)second) {
            DListRemove(second); // blocks
            DListRemove((MemoryBlockFree*)second); // free_blocks
            first->size += second->size + sizeof(MemoryBlock);
            mem_fill_zero((u8*)second, sizeof(MemoryBlock));
            merged = TRUE;
            memory->merged_count++;
        }
    }
    
    return merged;
}

u8* GMemAlloc(GMem *memory, psize size) {
    Assert(size > 0);
    
    psize minimum_size = sizeof(void*) * 2;
    
    if (size < minimum_size) {
        size = minimum_size;
    }
    
    MemoryBlock *block = 0;
    
    MemoryBlockFree *free_blocks = &memory->free_blocks;
    // find and resuse free block
    if (DListNotEmpty(free_blocks)) {
        for (MemoryBlockFree *nf = free_blocks->next; nf != free_blocks; nf = nf->next) {
            
            Assert(nf->block.size >= minimum_size);
            if (size <= nf->block.size) {
                block = &nf->block;
                
                if (!split_if_posible(nf, size)) 
                {
                    memory->free_blocks_count--;
                }
                
                DListRemove(nf);
                
                break;
            }
        }
    }
    
    // new block if not found any free block
    if (!block) {
        block = (MemoryBlock*)(memory->mem + memory->mem_used);
        block->mem = (u8*)block + sizeof(MemoryBlock);
        block->size = size;
        
        memory->mem_used += size + sizeof(MemoryBlock);
        memory->block_count++;
        DListInsertBack(&memory->blocks, block);
    }
    
    Assert(memory->mem_used <= memory->mem_size);
    block->status = MEMORY_BLOCK_USED;
    
    return block->mem;
}

void GMemFree(GMem *memory, void *data) {
    MemoryBlockFree *block_node = (MemoryBlockFree*)((u8*)data - sizeof(MemoryBlock));
    MemoryBlock *block = &block_node->block;
    block->status = MEMORY_BLOCK_FREE;
    
    // mem zero
    psize num_bytes = block->size;
    u8 *u8_data = (u8*)data;
    mem_fill_zero(u8_data, num_bytes);
    
    DListInsert(&memory->free_blocks, block_node);
    memory->free_blocks_count++;
    
    if (merge_block_if_posible(memory, block, block->next)) {
        --memory->free_blocks_count;
    }
    if (merge_block_if_posible(memory, block->prev, block)) {
        --memory->free_blocks_count;
    }
}


u8* GMemRealloc(GMem *memory, void* old_mem, psize size) {
    u8* new_mem = (u8*)old_mem;
    
    if (size) {
        if (old_mem) {
            MemoryBlock *block = (MemoryBlock*)(new_mem - sizeof(MemoryBlock));
            Assert(block->status == MEMORY_BLOCK_USED);
            if (size > block->size) {
                MemoryBlock *last_block = memory->blocks.prev;
                if (block == last_block) { // just expand if it's an ending block
                    Assert(block->mem + block->size == memory->mem + memory->mem_used);
                    memory->mem_used += (size - block->size);
                    block->size = size;
                    ++memory->expand_count;
                } else {
                    new_mem = GMemAlloc(memory, size);
                    mem_copy(old_mem, new_mem, block->size);
                    
                    GMemFree(memory, old_mem);
                }
            }
        } else {
            new_mem = GMemAlloc(memory, size);
        }
    }
    
    
    return new_mem;
}

