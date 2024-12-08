#include "defines.h"
#include "os.h"
//#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include "str.h"

void *OS_MemoryAlloc(psize size)
{
    return VirtualAlloc(0, size,
                        MEM_COMMIT | MEM_RESERVE,
                        PAGE_READWRITE);
}

void *OS_MemoryReserve(psize size)
{
    return VirtualAlloc(0, size, MEM_RESERVE, PAGE_NOACCESS);
}

void *OS_MemoryCommit(void *vmem, psize size)
{
    return VirtualAlloc(vmem, size, MEM_COMMIT, PAGE_READWRITE);
}

void OS_MemoryFree(void *mem)
{
    VirtualFree(mem, 0,MEM_RELEASE);
}

void OS_MemoryZero(void *mem, psize size)
{
    ZeroMemory(mem, size);
}

void OS_MemoryMove(void *dest, void *src, psize size)
{
    MoveMemory(dest, src, size);
}
void OS_MemoryCopy(void *dest, void *src, psize size)
{
    CopyMemory(dest, src, size);
}

void OS_MemoryFill(void *dest, psize size, u8 val)
{
    FillMemory(dest, size, val);
}

psize OS_MemoryCompare(void *a, void *b, psize size)
{
    u64 match_count = 0;
    u8 * ma = (u8*)a;
    u8 * mb = (u8*)b;
    while (ma++ == mb++ && match_count < size)
    {
        ++match_count;
    }
    
    return match_count;
}

b8 OS_GetFileInfo(CString file_name,  FileAttribute *stat)
{
    WIN32_FILE_ATTRIBUTE_DATA file_attribute = {0};
    BOOL ret = GetFileAttributesEx((LPCSTR)file_name.str, GetFileExInfoStandard, &file_attribute);
    
    OS_MemoryCopy(&stat->last_write, &file_attribute.ftLastWriteTime, sizeof(stat->last_write));
    OS_MemoryCopy(&stat->last_access, &file_attribute.ftLastAccessTime, sizeof(stat->last_write));
    return (ret != 0);
}

void *OS_ReadEntireFile(CString file_name, psize *size)
{
    u8 *file_contents = 0;
    
    HANDLE hFile = CreateFile((LPCSTR)file_name.str,               // file to open
                              GENERIC_READ,          // open for reading
                              FILE_SHARE_READ,       // share for reading
                              NULL,                  // default security
                              OPEN_EXISTING,         // existing file only
                              FILE_ATTRIBUTE_NORMAL, // normal file
                              NULL);
    if (hFile != INVALID_HANDLE_VALUE) 
    { 
        u32 file_size = GetFileSize( hFile, NULL);
        file_contents = (u8*)OS_MemoryAlloc(file_size);
        if (file_contents)
        {
            BOOL success = 0;
            u32 total_read_bytes = 0;
            do {
                u32 read_bytes = 0;
                success = ReadFile(hFile, file_contents + read_bytes, file_size - read_bytes, (LPDWORD)&read_bytes, 0);
                total_read_bytes += read_bytes;
            } while (total_read_bytes < file_size && success);
            
            *size = total_read_bytes;
        }
        
        CloseHandle(hFile);
    }
    
    return file_contents;
}

b8 OS_CopyFile(CString dest, CString source, b8 overwrite)
{
    return (0 != CopyFile((LPCSTR)source.str, (LPCSTR)dest.str, overwrite ? FALSE : TRUE)); 
}

u32 OS_PrintConsole(CString s)
{
    HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written = 0;
    WriteFile(std_out, s.str, s.size, &written, 0);
    
    return written;
}