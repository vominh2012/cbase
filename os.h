/* date = October 6th 2024 0:04 pm */

#ifndef OS_H
#define OS_H

void *MemoryAlloc(psize size);
void MemoryFree(void *mem);
void MemoryZero(void *mem, psize size);
void MemoryMove(void *dest, void *src, psize size);
void MemoryCopy(void *dest, void *src, psize size);
void MemoryFill(void *dest, psize size, u8 val);

// this will returns the number of bytes in the two blocks that match
u64 MemoryCompare(void *a, void *b, psize size);

typedef struct {
    u64 last_write;
    u64 last_access;
    u64 file_size;
} FileAttribute;

b8 GetFileInfo(CString file_name, FileAttribute *stat);
b8 CopyFileB(CString dest, CString source, b8 overwrite);

u32 PrintConsole(void *buff, u32 size);

#endif //OS_H
