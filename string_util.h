/* date = October 27th 2024 10:08 am */

#ifndef STRING_UTIL_H
#define STRING_UTIL_H

inline int SCompare(char *a, char *b, int max_length)
{
    return strncmp(a, b, max_length);
}

inline char * SCopy(char *dest, char *src, int length)
{
    return strncpy(dest, src, length);
}

inline u32 SLen(char *s)
{
    return (u32)strlen(s);
}


inline char *SFind(char *s, char c)
{
    while (*s != 0 && *s != c) {
        s++;
    }
    
    if (*s == c) {
        return s;
    }
    
    return 0;
}

static void SEatEndSpace(char *s)
{
    int length = SLen(s);
    if (length > 0)
    {
        char *c = s + length - 1;
        while (c > s && (*c == ' ' || *c == '\r' || *c == '\n' || *c == '\t'))
        {
            *c = 0;
            --c;
        }
    }
}

static bool SBeginWith(char *a, char *b)
{
    int i = 0;
    while (b[i])
    {
        if  (a[i] != b[i]) return false;
        i++;
    }
    
    return true;
}


#endif //STRING_UTIL_H
