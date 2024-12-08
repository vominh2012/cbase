/* date = October 6th 2024 0:04 pm */

#ifndef OS_H
#define OS_H

void *OS_MemoryAlloc(psize size);
void *OS_MemoryReserve(psize size);
void *OS_MemoryCommit(void *vmem, psize size);

void OS_MemoryFree(void *mem);
void OS_MemoryZero(void *mem, psize size);
void OS_MemoryMove(void *dest, void *src, psize size);
void OS_MemoryCopy(void *dest, void *src, psize size);
void OS_MemoryFill(void *dest, psize size, u8 val);

// this will returns the number of bytes in the two blocks that match
psize OS_MemoryCompare(void *a, void *b, psize size);

typedef struct {
    u64 last_write;
    u64 last_access;
    u64 file_size;
} FileAttribute;

b8 OS_GetFileInfo(CString file_name, FileAttribute *stat);
b8 OS_CopyFile(CString dest, CString source, b8 overwrite);

void *OS_ReadEntireFile(CString file_name, psize *size);

u32 OS_PrintConsole(CString text);

#endif //OS_H
