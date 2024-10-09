#include "defines.h"
#include "os.h"
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include "str.h"

void *MemoryAlloc(u64 size)
{
    return VirtualAlloc(0, (size_t)size,
                        MEM_COMMIT | MEM_RESERVE,
                        PAGE_READWRITE);
}

void MemoryFree(void *mem)
{
    VirtualFree(mem, 0,MEM_RELEASE);
}

void MemoryZero(void *mem, u64 size)
{
    ZeroMemory(mem, (size_t)size);
}

void MemoryMove(void *dest, void *src, u64 size)
{
    MoveMemory(dest, src, size);
}
void MemoryCopy(void *dest, void *src, u64 size)
{
    CopyMemory(dest, src, size);
}

void MemoryFill(void *dest, u64 size, u8 val)
{
    FillMemory(dest, size, val);
}

u64 MemoryCompare(void *a, void *b, u64 size)
{
    return RtlCompareMemory(a, b, size);
}

u32 PrintConsole(void *buff, u32 size)
{
    HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written = 0;
    WriteFile(std_out, buff, size, &written, 0);
    
    return written;
}