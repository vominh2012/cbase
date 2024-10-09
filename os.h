/* date = October 6th 2024 0:04 pm */

#ifndef OS_H
#define OS_H

void *MemoryAlloc(u64 size);
void MemoryFree(void *mem);
void MemoryZero(void *mem, u64 size);
void MemoryMove(void *dest, void *src, u64 size);
void MemoryCopy(void *dest, void *src, u64 size);
void MemoryFill(void *dest, u64 size, u8 val);

// this will returns the number of bytes in the two blocks that match
u64 MemoryCompare(void *a, void *b, u64 size);

u32 PrintConsole(void *buff, u32 size);

#endif //OS_H
