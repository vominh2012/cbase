/* date = April 27th 2024 0:53 pm */

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "Windows.h"

typedef void* allocator_alloc_sig(void *thiz, size_t size, u32 flags);
typedef void* allocator_realloc_sig(void *thiz, void *ptr,size_t size, u32 flags);
typedef void allocator_free_sig(void *thiz, void *mem);


enum MemoryBlockStatus {
    MEMORY_BLOCK_NULL,
    MEMORY_BLOCK_FREE,
    MEMORY_BLOCK_USED
};

typedef struct MemoryBlock {
    u8 *mem;
    u64 size;
    u32 status;
    
    MemoryBlock *next;
    MemoryBlock *prev;
} MemoryBlock;

typedef struct MemoryBlockFree {
    MemoryBlock block;
    MemoryBlockFree *next;
    MemoryBlockFree *prev;
} MemoryBlockFree;

typedef struct Memory {
    u8 *persist_mem;
    u64 persist_mem_size;
    
    u8 *mem;
    u64 mem_used;
    u64 mem_size;
    
    u64 free_blocks_count;
    MemoryBlockFree free_blocks;
    
    u64 block_count;
    MemoryBlock blocks;
    
    // statictic
    u64 merged_count;
    u64 expand_count;
    
    
    
    bool split_if_posible(MemoryBlockFree *n, u64 size) {
        bool splited = false;
        
        u64 remaining_bytes = n->block.size - size;
        if (remaining_bytes >= sizeof(MemoryBlockFree)) {
            
            n->block.size = size;
            
            MemoryBlockFree *free_block = (MemoryBlockFree *)(n->block.mem + size);
            
            MemoryBlock *block = &free_block->block;
            block->mem = (u8*)block + sizeof(MemoryBlock);
            block->size = remaining_bytes - sizeof(MemoryBlock);
            block->status = MEMORY_BLOCK_FREE;
            
            dll_insert(n, free_block);
            dll_insert(&n->block, block);
            
            splited = true;
        }
        
        return splited;
    }
    
    u8* alloc(u64 size) {
        assert(size > 0);
        
        u64 minimum_size = sizeof(MemoryBlockFree::next) + sizeof(MemoryBlockFree::prev);
        
        if (size < minimum_size) {
            size = minimum_size;
        }
        
        MemoryBlock *block = 0;
        
        // find and resuse free block
        if (dll_not_empty(&free_blocks)) {
            for (MemoryBlockFree *nf = free_blocks.next; nf != &free_blocks; nf = nf->next) {
                
                assert(nf->block.size >= minimum_size);
                if (size <= nf->block.size) {
                    block = &nf->block;
                    
                    if (!split_if_posible(nf, size)) 
                    {
                        free_blocks_count--;
                    }
                    
                    dll_remove(nf);
                    
                    break;
                }
            }
        }
        
        // new block if not found any free block
        if (!block) {
            block = (MemoryBlock*)(mem + mem_used);
            block->mem = (u8*)block + sizeof(MemoryBlock);
            block->size = size;
            
            mem_used += size + sizeof(MemoryBlock);
            block_count++;
            dll_insert_back(&blocks, block);
        }
        
        assert(mem_used <= mem_size);
        block->status = MEMORY_BLOCK_USED;
        
        return block->mem;
    }
    
    u8* realloc(void* old_mem, u64 size) {
        u8* new_mem = (u8*)old_mem;
        
        if (size) {
            if (old_mem) {
                MemoryBlock *block = (MemoryBlock*)(new_mem - sizeof(MemoryBlock));
                assert(block->status == MEMORY_BLOCK_USED);
                if (size > block->size) {
                    MemoryBlock *last_block = blocks.prev;
                    if (block == last_block) { // just expand if it's an ending block
                        assert(block->mem + block->size == mem + mem_used);
                        mem_used += (size - block->size);
                        block->size = size;
                        ++expand_count;
                    } else {
                        new_mem = alloc(size);
                        mem_copy(old_mem, new_mem, block->size);
                        
                        this->free(old_mem);
                    }
                }
            } else {
                new_mem = alloc(size);
            }
        }
        
        
        return new_mem;
    }
    
    void mem_fill_zero(u8* u8_data, u64 num_bytes)
    {
        while (num_bytes-- > 0) {
            *(u8_data++) = 0;
        }
    }
    
    bool merge_block_if_posible(MemoryBlock *first, MemoryBlock *second) {
        bool merged = false;
        
        if (first->status == MEMORY_BLOCK_FREE && second->status == MEMORY_BLOCK_FREE) {
            // is contiguous
            if ((u8*)(first->mem) + first->size == (u8*)second) {
                dll_remove(second); // blocks
                dll_remove((MemoryBlockFree*)second); // free_blocks
                first->size += second->size + sizeof(MemoryBlock);
                mem_fill_zero((u8*)second, sizeof(MemoryBlock));
                merged = true;
                merged_count++;
            }
        }
        
        return merged;
    }
    
    void free(void *data) {
        MemoryBlockFree *block_node = (MemoryBlockFree*)((u8*)data - sizeof(MemoryBlock));
        MemoryBlock *block = &block_node->block;
        block->status = MEMORY_BLOCK_FREE;
        
        // mem zero
        u64 num_bytes = block->size;
        u8 *u8_data = (u8*)data;
        mem_fill_zero(u8_data, num_bytes);
        
        dll_insert(&free_blocks, block_node);
        free_blocks_count++;
        
        if (merge_block_if_posible(block, block->next)) {
            --free_blocks_count;
        }
        if (merge_block_if_posible(block->prev, block)) {
            --free_blocks_count;
        }
    }
} Memory;

void memory_init(Memory *mem, u64 persist_mem_size, u64 app_mem_size) {
    dll_init(&mem->free_blocks);
    dll_init(&mem->blocks);
    
    u64 total_mem_size = persist_mem_size + app_mem_size;
    
    mem->persist_mem = (u8*)VirtualAlloc(0, (size_t)total_mem_size,
                                         MEM_COMMIT | MEM_RESERVE,
                                         PAGE_READWRITE);
    mem->persist_mem_size = persist_mem_size;
    
    mem->mem = mem->persist_mem + mem->persist_mem_size;
    mem->mem_size = app_mem_size;
    
    assert(mem != 0);
}


#define struct_alloc(allocator, type) (type*)(allocator)->alloc(sizeof(type))
#define struct_free(allocator, n) if (allocator) (allocator)->free(n)

#endif //ALLOCATOR_H
