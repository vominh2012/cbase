#define _CRT_RAND_S
#define _CRTDBG_MAP_ALLOC
#include "base.h"
#include <time.h>
#include <stdlib.h>
#include <crtdbg.h>

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
    MArena *arena = ArenaNew(MB(1));
    
    ArenaPush(arena, MB(1));
    ArenaPush(arena, MB(1));
    ArenaPush(arena, MB(1));
    ArenaPop(arena, MB(1));
    ArenaPop(arena, MB(1));
    ArenaPop(arena, MB(1));
    
    // pop back to original arena
    Assert(arena->next == arena);
    
    
    u8* mem = ArenaPush(arena, MB(1));
    Assert(mem != 0);
    Assert(arena->next->size >= MB(1));
    ArenaPop(arena, MB(1));
    
    psize prev_arena_size = arena->size;
    
    MTempArena temp = BeginTempArena(arena);
    ArenaPush(arena, KB(1));
    ArenaPush(arena, MB(1));
    ArenaPush(arena, MB(2));
    EndTempArena(&temp);
    
    Assert(prev_arena_size == arena->size);
    
    ArenaPush(arena, MB(1));
    ArenaPop(arena, MB(1));
    
    ArenaDestroy(arena);
}

void test_arena_random()
{
    srand ((u32)time(NULL));
    
    const int max_size = MB(128);
    const int max_loop = 10;
    
    psize arena_size;
    psize max_push;
    
    rand_s((u32*)&arena_size);
    rand_s((u32*)&max_push);
    
    arena_size = arena_size % max_size;
    MArena *arena = ArenaNew(arena_size);
    
    
    max_push= max_push % max_size;
    for (int i = 0; i < max_loop; ++i)
    {
        psize push_size;
        rand_s((u32*)&push_size);
        push_size = push_size % max_push;
        
        u8 *data = ArenaPush(arena, push_size);
        OS_MemoryFill(data, push_size, 1);
        ArenaPop(arena, push_size);
    }
    
    u32 arena_list_count = 1;
    MArena *iter = arena->next;
    while (iter != arena)
    {
        arena_list_count++;
        iter = iter->next;
    }
    Assert(arena_list_count == 1);
    
    for (int i = 0; i < max_loop; ++i)
    {
        psize push_size;
        rand_s((u32*)&push_size);
        push_size = push_size % max_push;
        MTempArena temp = BeginTempArena(arena);
        u8 *data =ArenaPush(temp.arena, push_size);
        OS_MemoryFill(data, push_size, 1);
        EndTempArena(&temp);
    }
    
    ArenaDestroy(arena);
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
    MArena *arena = ArenaNew(GB(1));
    
    CString a = StringLit("abcdef\n");
    
    CString formated = StringFormat(arena, "%s %d %d\n", a.str, 11, 12);
    OS_PrintConsole(formated);
    
    CString str_list = StringLit("1, 2, 3, 4, 5\n");
    
    StringList list;
    DListInit((StringNode*)&list);
    StringSplit(arena, &list, str_list, ',');
    
    for(StringNode *node = list.next; node != (StringNode*)&list; node = node->next)
    {
        OS_PrintConsole(node->s);
        OS_PrintConsole(StringLit("\n"));
    }
    
    //String b = StringCopyC(&arena, a);
    //String c = StringAlloc(&arena, KB(1));c;
    
    ArenaDestroy(arena);
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
    test_arena_random();
    test_general_purpose_memory();
    test_string();
    
    
    print("Test end\n");
    malloc(10);
    _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtDumpMemoryLeaks();
}