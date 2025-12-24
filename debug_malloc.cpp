// debug_malloc.cpp
// compile by using: cl /EHsc /W4 /D_DEBUG /MDd debug_malloc.cpp
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#include <iostream>

int main()
{
    std::cout << "Hello World!\n";
    
    int* x = (int*)malloc(sizeof(int));
    
    *x = 7;
    
    printf("%d\n", *x);
    
    _CrtMemState s1;
    _CrtMemState s2;
    _CrtMemState s3;
    _CrtMemCheckpoint( &s1 );
    x = (int*)calloc(3, sizeof(int));
    x[0] = 7;
    x[1] = 77;
    x[2] = 777;
    
    printf("%d %d %d\n", x[0], x[1], x[2]);
    // memory allocations take place here
    _CrtMemCheckpoint( &s2 );
    
    if ( _CrtMemDifference( &s3, &s1, &s2) )
        _CrtMemDumpStatistics( &s3 );
    
    _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR );
    //_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG); 
    _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDERR );
    _CrtDumpMemoryLeaks();
}