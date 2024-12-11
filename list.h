/* date = October 19th 2024 11:37 am */

#ifndef LIST_H
#define LIST_H

// double circular linked list
// (s)entinal, (n)ode, (t)ype 
// require: need to define next and prev pointer in your struct

typedef struct DListNode {
    struct DListNode *next;
    struct DListNode *prev;
} DListNode;

#define DListInit(s) (s)->next=s,(s)->prev=s
#define DListBegin(s) (s)->next
#define DListEnd(s) (s)->prev
#define DListIsEnd(s, n) (n == s)
#define DListNext(n) n->next
#define DListIsEmpty(s) ((s)->next) == (s)
#define DListNotEmpty(s) ((s)->next) != (s)

#define DListInsert(n, val) (val)->prev=(n), (val)->next=(n)->next, (val)->prev->next=val, (val)->next->prev=(val)
#define DListInsertBack(s, n) DListInsert((s)->prev, n)
#define DListInsertFront(s, n) DListInsert(s, n)
#define DListRemove(n) (n)->prev->next=(n)->next, (n)->next->prev=(n)->prev, (n)->next=(n)->prev=0

typedef struct SListNode {
    struct SListNode *next;
} SListNode;

#define SListInit(s) (s)->next=s
#define SListBegin(s) (s)->next
#define SListIsEnd(s, n) (n == s)
#define SListNext(n) n->next
#define SListIsEmpty(s) ((s)->next) == (s)
#define SListNotEmpty(s) ((s)->next) != (s)

#define SListStackPush(n, val) (val)->next=(n)->next,(n)->next=(val)
#define SListStackPop(n) (n)->next = (n)->next->next

#endif //LIST_H
