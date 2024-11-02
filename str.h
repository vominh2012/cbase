/* date = October 6th 2024 5:32 pm */

#ifndef STR_H
#define STR_H

// C constant string
// noted: keep this same structure with String and you are fine
typedef struct CString {
    u8 *str;
    psize size;
} CString;

typedef struct String {
    u8 *str;
    psize size;
} String;

#ifdef __cplusplus
#define StringLit(s) CString { (u8*)(s), sizeof(s) - 1 }
#define StringMake(s) CString { (u8*)(s), strlen(s) }
#else
#define StringLit(s) (CString) { (u8*)(s), sizeof(s) - 1 }
#define StringMake(s) (CString) { (u8*)(s), strlen(s) }
#endif
#define StringExpand(s) (s).str, (u64)(s).size

//String StringAlloc(struct Arena *arena, psize size);
//String StringCopy(struct Arena *arena, String src);
//String StringCopyC(struct Arena *arena, CString src);

#endif //STR_H
