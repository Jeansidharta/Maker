#include<stdio.h>
#include<stdlib.h>

typedef struct vector_t{
   void** data;
   int size;
   int dataSize;
} vector_t;

vector_t* vector_create(int dataSize){
   vector_t* vector = (vector_t*)calloc(1, sizeof(vector_t));
   vector->dataSize = dataSize;
   vector->data = (void**)malloc(sizeof(void*));
   vector->data[0] = NULL;
   return vector;
}

void vector_delete(vector_t* vector){
   free(vector->data);
   free(vector);
}

void vector_push(vector_t* vec, void* data){
   vec->data = (void**) realloc(vec->data, sizeof(void*) * (vec->size+2));
   vec->data[vec->size] = data;
   vec->size ++;
   vec->data[vec->size] = NULL;
}

void* vector_pop(vector_t* vec){
   void* returnValue = vec->data[vec->size-1];
//   vec->data = (void**)realloc(vec->data, sizeof(void*) * vec->size);
   vec->data[--vec->size] = '\0';
   return returnValue;
}

void vector_bubbleSort(vector_t* vector, int compare(void*, void*)){
   int aux, aux1;
   for(aux = vector->size - 1;aux >= 0; aux --){
      for(aux1 = 0; aux1 < aux; aux1 ++){
         if(compare(vector->data[aux1], vector->data[aux1 + 1]) == 0){
            void* buffer = vector->data[aux1];
            vector->data[aux1] = vector->data[aux1 + 1];
            vector->data[aux1 + 1] = buffer;
         }
      }
   }
}
