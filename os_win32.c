#include "defines.h"
#include "os.h"
//#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include "str.h"

void *MemoryAlloc(psize size)
{
    return VirtualAlloc(0, size,
                        MEM_COMMIT | MEM_RESERVE,
                        PAGE_READWRITE);
}

void MemoryFree(void *mem)
{
    VirtualFree(mem, 0,MEM_RELEASE);
}

void MemoryZero(void *mem, psize size)
{
    ZeroMemory(mem, (size_t)size);
}

void MemoryMove(void *dest, void *src, psize size)
{
    MoveMemory(dest, src, size);
}
void MemoryCopy(void *dest, void *src, psize size)
{
    CopyMemory(dest, src, size);
}

void MemoryFill(void *dest, psize size, u8 val)
{
    FillMemory(dest, size, val);
}

u64 MemoryCompare(void *a, void *b, psize size)
{
    u64 match_count = 0;
    u8 * ma = a;
    u8 * mb = b;
    while (ma++ == mb++ && match_count < size)
    {
        ++match_count;
    }
    
    return match_count;
}

b8 GetFileInfo(CString file_name,  FileAttribute *stat)
{
    WIN32_FILE_ATTRIBUTE_DATA file_attribute = {0};
    BOOL ret = GetFileAttributesEx(file_name.str, GetFileExInfoStandard, &file_attribute);
    
    MemoryCopy(&stat->last_write, &file_attribute.ftLastWriteTime, sizeof(stat->last_write));
    MemoryCopy(&stat->last_access, &file_attribute.ftLastAccessTime, sizeof(stat->last_write));
    return (ret != 0);
}

b8 CopyFileB(CString dest, CString source, b8 overwrite)
{
    return (0 != CopyFile(source.str, dest.str, overwrite ? FALSE : TRUE)); 
}

u32 PrintConsole(void *buff, u32 size)
{
    HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written = 0;
    WriteFile(std_out, buff, size, &written, 0);
    
    return written;
}