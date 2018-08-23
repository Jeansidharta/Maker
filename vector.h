#ifndef _LIBRARY_VECTOR
#define _LIBRARY_VECTOR


typedef struct vector_t{
   void** data;
   int size;
   int dataSize;
} vector_t;

vector_t* vector_create(int dataSize);
void vector_delete(vector_t* vector);
void vector_push(vector_t* vec, void* data);
void* vector_pop(vector_t* vec);
void vector_bubbleSort(vector_t* vector, int compare(void*, void*));
#endif
