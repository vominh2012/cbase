#include "str.h"

// #define STB_SPRINTF_NOFLOAT
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

CString StringTrimSpace(CString s) {
    while(s.size > 0 && *s.str == ' ')
    {
        ++s.str;
        --s.size;
    }
    
    return s;
}

CString StringFormat(MArena *arena, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    
    CString ret =  {0};
    
    int count = stbsp_vsnprintf(0, 0, format, args) + 1;
    ret.str = ArenaPush(arena, count);
    
    if (ret.str)
    {
        // cannot pass the variadic arguments to a variadic function so you v version
        // noted: stb alway write null terminated at the end
        int written = stbsp_vsnprintf((char*)ret.str, count, format, args);
        
        Assert(written == count - 1);
        
        ret.size = written;
    }
    else 
    {
        Assert(0); // oops
    }
    
    va_end(args);
    
    return ret;
}

void StringSplit(struct MArena *arena, StringList *list, CString s, char separator)
{
    u8* string_end = s.str + s.size;
    u8* token_start = s.str;
    u8* token_end = s.str;
    while(token_end < string_end)
    {
        while (token_end < string_end && *token_end != separator)
        {
            token_end++;
        }
        
        if (token_end != token_start)
        {
            CString token = {token_start, (u32)(token_end - token_start) };
            StringNode *node = ArenaPushStruct(arena, StringNode);
            node->s = token;
            DListInsertBack(list, node);
            list->count++;
        }
        
        ++token_end;
        token_start = token_end;
    }
}
