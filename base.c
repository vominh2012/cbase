#include "base.h"

#ifdef __cplusplus 
extern "C" {
#endif 
    
#ifdef  _WIN32
#include "os_win32.c"
#endif
    
#include "hotload.c"
#include "str.c"
#include "memory.c"
#include "allocator.c"
    
#ifdef __cplusplus 
}
#endif