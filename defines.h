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

typedef void (*fnptr)(void);

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

#define KB(n) ((n) * 1024)
#define MB(n) ((n) * KB(1024))
#define GB(n) ((n) * MB(1024))

#define Thousand(x) ((x)*1000)
#define Million(x)  ((x)*1000000)
#define Billion(x)  ((x)*1000000000)

#define HasFlag(fi,fl) (((fi)&(fl))!=0)
#define HasAllFlag(fi,fl) (((fi)&(fl))==(fl))
#define AddFlag(fi,fl) ((fi)|=(fl))
#define RemFlag(fi,fl) ((fi)&=(~(fl)))
#define MovFlag(fi1,fl1,fi2,fl2) ((HasFlag(fi1,fl1))?(AddFlag(fi2,fl2)):(fi2))

#ifndef Assert
#define Assert(exp) if (!(exp)) (*((int*)0) = 0)
#endif
#define ArrayCount(arr) (sizeof(arr) / sizeof(arr[0]))
#define Swap(x, y, type) {  type temp = x; x = y; y = temp; }

#define BMin(a,b) (((a)<(b))?(a):(b))
#define BMax(a,b) (((a)>(b))?(a):(b))
#define Lerp(v0, v1,  t) ((v0) + (t) * ((v1) - (v0)))
#define SafeExec(func, param) { if (param) func(param); }
#define IsPowerOfTwo(x) ((x & (x-1)) == 0)
#define AlignSize(size, align) ((size) + ((align) - ((size) % (align))))
#define Abs(x) ((x) < 0 ? -(x) : (x))

#define MIN_INT (-2147483647 - 1)
#define MAX_INT 2147483647
#define MAX_U32 0xffffffff

#define StructAlloc(type, size) (type*)calloc(sizeof(type), size);

#endif //DEFINES_H
