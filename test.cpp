#include "base.h"

typedef struct A {
    struct A *next;
    struct A *prev;
} A;

#define print(msg) OS_PrintConsole(StringLit(msg))

void test_os()
{
    char msg[] = "hello world\n";
    const u32 size = ArrayCount(msg) - 1; // null terminated
    
    u32 result = OS_PrintConsole(StringLit(msg));
    Assert(result == size);
}

void test_memory_pool()
{
    MemoryPool mem_pool;
    MemoryPoolInit(&mem_pool, sizeof(u32), 3);
    int *ma = (int*)MemoryPoolGet(&mem_pool);
    int *mb = (int*)MemoryPoolGet(&mem_pool);
    int *mc = (int*)MemoryPoolGet(&mem_pool);
    int *md = (int*)MemoryPoolGet(&mem_pool);
    Assert(ma);
    Assert(mb);
    Assert(mc);
    Assert(md == 0);
    
    MemoryPoolRelease(&mem_pool, ma);
    ma = (int*)MemoryPoolGet(&mem_pool);
    Assert(ma);
}

void test_arena() 
{
    MArena arena;
    ArenaInit(&arena, KB(1));
    
    ArenaPush(&arena, MB(1));
    ArenaPush(&arena, MB(1));
    ArenaPush(&arena, MB(1));
    ArenaPop(&arena, MB(1));
    ArenaPop(&arena, MB(1));
    ArenaPop(&arena, MB(1));
    
    u8* mem = ArenaPush(&arena, MB(1));
    Assert(mem != 0);
    Assert(arena.next->size >= MB(1));
    ArenaPop(&arena, MB(1));
    
    ArenaDestroy(&arena);
}

void test_general_purpose_memory()
{
    GMem mem = {0};
    GMemInit(&mem, MB(10));
    
    void *buff1 = GMemAlloc(&mem, 10);
    void *buff2 = GMemAlloc(&mem, 10);
    void *buff3 = GMemAlloc(&mem, 10);
    Assert(buff1);
    Assert(buff2);
    Assert(buff3);
    
    Assert(mem.merged_count == 0);
    GMemFree(&mem, buff1);
    GMemFree(&mem, buff2);
    GMemFree(&mem, buff3);
    Assert(mem.merged_count == 2);
}

void test_string() 
{
    MArena arena;
    ArenaInit(&arena, GB(1));
    
    CString a = StringLit("abcdef\n");
    
    CString formated = StringFormat(&arena, "%s %d %d\n", a.str, 11, 12);
    OS_PrintConsole(formated);
    
    CString str_list = StringLit("1, 2, 3, 4, 5\n");
    
    StringList list;
    DListInit((StringNode*)&list);
    StringSplit(&arena, &list, str_list, ',');
    
    for(StringNode *node = list.next; node != (StringNode*)&list; node = node->next)
    {
        OS_PrintConsole(node->s);
        OS_PrintConsole(StringLit("\n"));
    }
    
    //String b = StringCopyC(&arena, a);
    //String c = StringAlloc(&arena, KB(1));c;
    
    //ArenaDestroy(&arena);
}

void test_list()
{
    A sentinal;
    DListInit(&sentinal);
    
    Assert(DListIsEmpty(&sentinal));
    A node_1;
    A node_2;
    DListInsertBack(&sentinal, &node_1);
    DListInsertFront(&sentinal, &node_2);
    Assert(DListNotEmpty(&sentinal));
}

void main() {
    print("Test start\n");
    test_list();
    test_os();
    test_memory_pool();
    test_arena();
    test_general_purpose_memory();
    test_string();
    
    
    print("Test end\n");
}