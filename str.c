#include "str.h"

#define STB_SPRINTF_NOFLOAT
#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"

static void MemCopy(void *dest, void *src, psize size) {
    u64 copy_bytes = size; 
    u8 *u8_src = (u8*)src;
    u8 *u8_dest = (u8*)dest;
    while (copy_bytes--) {
        *(u8_dest++) = *(u8_src++);
    }
}

String StringAlloc(Arena *arena, psize size) 
{
    String result = { ArenaPush(arena, size), size };
    return result;
}

String StringCopyC(Arena *arena, CString src)
{
    String result = { ArenaPush(arena, src.size), src.size };
    MemCopy(result.str, src.str, src.size);
    return result;
}

String StringCopy(Arena *arena, String src)
{
    return StringCopyC(arena, (CString){ src.str, src.size  });
}

