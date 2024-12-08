/* date = October 30th 2024 6:42 pm */

#ifndef LOG_H
#define LOG_H
#include "stb_sprintf.h"

#define Log(level, format, ...) {\
char buff[1024] = {0};\
int written = stbsp_snprintf(buff, ArrayCount(buff), "%s:%d: %s: ",__FILE__, __LINE__, (level));\
written += stbsp_snprintf(buff + written, ArrayCount(buff) - written, format, ##__VA_ARGS__);\
buff[written] = '\n';\
CString sbuff = {(u8*)buff, (u32)written + 1};\
OS_PrintConsole(sbuff); \
}

#define LogInfo(format, ...) { Log("Info", format, ##__VA_ARGS__); }
#define LogWarning(format, ...) { Log("Warning", format, ##__VA_ARGS__); }
#define LogError(format, ...) { Log("Error", format, ##__VA_ARGS__); }

#endif //LOG_H
