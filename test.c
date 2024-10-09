#include "base.h"

typedef struct A {
    struct A *next;
    struct A *prev;
} A;

void main() {
    A sentinal;
    DListInit(&sentinal);
    
    Assert(DListIsEmpty(&sentinal));
    A node_1;
    A node_2;
    DListInsertBack(&sentinal, &node_1);
    DListInsertFront(&sentinal, &node_2);
    Assert(DListNotEmpty(&sentinal));
    
    
    Arena arena;
    ArenaInit(&arena, GB(1));
    void *mem = ArenaPush(&arena, MB(1));
    Assert(mem != 0);
    Assert(arena.size == MB(1));
    ArenaPop(&arena, MB(1));
    Assert(arena.size == 0);
    
    CString a = StringLit("abcdef\n");
    String b = StringCopyC(&arena, a);
    
    String c = StringAlloc(&arena, KB(1));
    
    u32 result = PrintConsole(StringExpand(a));
    Assert(result == a.size);
    result = PrintConsole(StringExpand(b));
    
    ArenaDestroy(&arena);
}