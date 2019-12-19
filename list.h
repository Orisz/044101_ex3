#ifndef _LIST_H_
#define _LIST_H_
#include "defs.h"

typedef struct List_* PList;
typedef void* pElement;
typedef void (*print_func)(pElement);
typedef void (*destroy_func)(pElement);
typedef pElement (*clone_func)(pElement);//mallocfail == NULL
typedef BOOL (*compare_func)(pElement, pElement);



PList ListCreate(clone_func, destroy_func, compare_func, print_func);
void ListDestroy(PList list);
Result ListAdd(PList list, pElement element);
Result ListRemove(PList list, pElement element);
pElement ListGetFirst(PList list);
pElement ListGetNext(PList list);
BOOL ListCompare(PList listA, PList listB);
void ListPrint(PList list);

#endif
