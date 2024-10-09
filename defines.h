/* date = October 6th 2024 0:20 pm */

#ifndef DEFINES_H
#define DEFINES_H
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef u8  b8;
typedef u32 b32;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef float f32;
typedef double f64;

#if defined(__ppc64__) || defined(__powerpc64__) || defined(__aarch64__) || defined(_M_X64) || defined(__x86_64__) || defined(__x86_64) || defined(__s390x__)
#define PLATFORM_X64
#else
#define PLATFORM_X86
#endif

#ifdef PLATFORM_X64
typedef u64 psize;
#else
typedef u32 psize;
#endif

#define func static

#define KB(n) ((n) * 1024)
#define MB(n) ((n) * KB(1024))
#define GB(n) ((n) * MB(1024))

#ifndef Assert
#define Assert(exp) if (!(exp)) (*((int*)0) = 0)
#endif
#define ArrayCount(arr) (sizeof(arr) / sizeof(arr[0]))
#define Swap(x, y, type) {  type temp = x; x = y; y = temp; }

#ifdef _cplusplus
#define Min(a,b) (((a)<(b))?(a):(b))
#define Max(a,b) (((a)>(b))?(a):(b))
#endif
#define Lerp(v0, v1,  t) ((v0) + (t) * ((v1) - (v0)))

#endif //DEFINES_H
