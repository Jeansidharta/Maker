#ifndef _LIBRARY_LIST
#define _LIBRARY_LIST


typedef struct listNode_taux{
   void* data;
   struct listNode_taux* next;
   struct listNode_taux* prev;
} listNode_t;

typedef struct{
   int size;
   listNode_t* start;
   listNode_t* end;
} list_t;

list_t* list_create();
listNode_t* listNode_create();
void list_push(list_t* list, void* number);
void list_unshift(list_t* list, void* number);
void* list_shift(list_t* list);
void* list_pop(list_t* list);
void* list_noPop(list_t* list);
void* list_noShift(list_t* list);
void* list_removeNode(list_t* list, listNode_t* node);
void* list_getElement(list_t* list, int index);
listNode_t* list_removeElement(list_t* list, int index);
void* list_removeValue(list_t* list, void* value);
int list_hasData(list_t* list, void* data, void* (*getData)(void*));
void list_delete(list_t* list);
void list_printInverted(list_t* list);
void list_print(list_t* list);
void list_printInvertedBySize(list_t* list);
void list_printBySize(list_t* list);
void list_printObject(list_t* list, void(*printObject)(void*));
void list_swapNodes(list_t* list, listNode_t* node1, listNode_t* node2);
void list_bubbleSort(list_t* list, int (*compare)(void*, void*));
void list_insertSortedElement(list_t* list, void* data, void* (*getData)(void*));
int list_checkIntegrity(list_t* list);
#endif
