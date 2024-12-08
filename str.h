/* date = October 6th 2024 5:32 pm */

#ifndef STR_H
#define STR_H

// C constant string
// noted: keep this same structure with String and you are fine
typedef struct CString {
    u8 *str;
    u32 size;
} CString;

typedef struct StringNode {
    CString s;
    struct StringNode *next;
    struct StringNode *prev;
} StringNode;

typedef struct StringList {
    CString unused;
    struct StringNode *next;
    struct StringNode *prev;
    u32 count;
} StringList;

#ifdef __cplusplus
#define StringLit(s) CString { (u8*)(s), sizeof(s) - 1 }
#define StringMake(s) CString { (u8*)(s), strlen(s) }
#else
#define StringLit(s) (CString) { (u8*)(s), sizeof(s) - 1 }
#define StringMake(s) (CString) { (u8*)(s), strlen(s) }
#endif
#define StringExpand(s) (s).str, (u64)(s).size

CString StringTrimSpace(CString s);

void StringSplit(struct MArena *arena, StringList *list, CString s, char separator);

CString StringFormat(struct MArena *arena, const char *format, ...);

#endif //STR_H
