/* date = October 6th 2024 5:32 pm */

#ifndef STR_H
#define STR_H

// C constant string
// noted: keep this same structure with String and you are fine
typedef struct CString {
    union {
        u8 *str;
        void *data;
    };
    u64 size;
} CString;

typedef struct String {
    union {
        u8 *str;
        void* data;
    };
    u64 size;
} String;

#define StringLit(s) (CString) { .str = (u8*)(s), .size = sizeof(s) - 1 }
#define StringMake(s) (CString) { .str = (u8*)(s), .size = strlen(s) }
#define StringExpand(s) (s).str, (u64)(s).size

String StringAlloc(struct Arena *arena, u64 size);
String StringCopy(struct Arena *arena, String src);
String StringCopyC(struct Arena *arena, CString src);

#endif //STR_H
