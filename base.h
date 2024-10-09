/* date = April 7th 2024 3:56 pm */

#ifndef C_BASE_API_H
#define C_BASE_API_H
#include "defines.h"
#include "os.h"
#include "memory.h"
#include "str.h"

// double circular linked list
// (s)entinal, (n)ode, (t)ype 
// require: need to define next and prev pointer in your struct
#define DListInit(s) (s)->next=s,(s)->prev=s
#define DListBegin(s) (s)->next
#define DListEnd(s) (s)->prev
#define DListIsEnd(s, n) (n == s)
#define DListNext(n) n->next
#define DListIsEmpty(s) ((s)->next) == (s)
#define DListNotEmpty(s) ((s)->next) != (s)

#define DListInsert(n, val) (val)->prev =n, (val)->next=(n)->next, (val)->prev->next=val, (val)->next->prev=val
#define DListInsertBack(s, n) DListInsert((s)->prev, n)
#define DListInsertFront(s, n) DListInsert(s, n)
#define DListRemove(n) (n)->prev->next=(n)->next, (n)->next->prev=(n)->prev, (n)->next=(n)->prev=0

#endif
