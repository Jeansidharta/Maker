#include<stdio.h>
#include<stdlib.h>

typedef struct string_t{
   char* ptr;
   char* base;
   int size;
} string_t;

string_t* string_create(char* arg){
   string_t* string;
   string = (string_t*) calloc(1, sizeof(string_t));
   if(arg == NULL)
      string->base = calloc(1, 1);
   else{
      do{
         string->base = (char*) realloc(string->base, ++(string->size));
         string->base[string->size - 1] = arg[string->size-1];
      } while(arg[string->size - 1] != '\0');
      string->base[--string->size] = '\0';
   }

   string->ptr = string->base;
   return string;
}

void string_delete(string_t* string){
   free(string->base);
   free(string);
}

string_t* string_duplicate(string_t* string){
   return string_create(string->ptr);
}

void string_realloc(string_t* string, size_t size){
   int dist = string->ptr - string->base;
   string->base = (char*) realloc(string->base, size + 1);
   string->ptr = string->base + dist;
   string->size = size - dist;
}

void string_addSize(string_t* string, int size){
   int dist = string->ptr - string->base;
   string->base = (char*) realloc(string->base, string->size + size + dist);
   string->ptr = string->base + dist;
   string->size += size;
}

void string_concatenateLiteral(string_t* a, char* c){
   int aux;
   string_t* b = string_create(c);

   string_realloc(a, ((a->ptr - a->base + a->size) + b->size));
   for(aux = 0; aux < b->size; aux ++){
      a->ptr[a->size - b->size + aux] = b->ptr[aux];
   }
   a->ptr[a->size] = '\0';
   free(b);
}

void string_concatenate(string_t* a, string_t* b){
   int aux;

   string_realloc(a, ((a->ptr - a->base + a->size) + b->size));
   for(aux = 0; aux < b->size; aux ++){
      a->ptr[a->size - b->size + aux] = b->ptr[aux];
   }
   a->ptr[a->size] = '\0';
}

void string_shift(string_t* string, int offset){
   if(string->ptr + offset < string->base || offset > string->size) return;
   string->size -= offset;
   string->ptr += offset;
}

void string_unshift(string_t* string){
   string->size += string->ptr - string->base;
   string->ptr = string->base;
}

int string_compare(string_t* string1, string_t* string2){
   int aux;
   for(aux = 0;aux < string1->size ;aux ++){
      if(string1->ptr[aux] != string2->ptr[aux]){
         return aux;
      }
   }
   if(string2->ptr[aux] != '\0') return aux;
   else return -1;
}

int string_compareLiteral(string_t* string1, char* string2){
   int aux;
   for(aux = 0;aux < string1->size ;aux ++){
      if(string1->ptr[aux] != string2[aux]){
         return aux;
      }
   }
   if(string2[aux] != '\0') return aux;
   else return -1;
}

int string_checkIntegrity(string_t* string){
   int aux;
   for(aux = 0;aux < string->size-1; aux ++){
      if(string->ptr[aux] == '\0') return -1;
   }
   if(string->ptr[string->size] != '\0') return -2;
   return 0;
}

void string_print(string_t* string){
   printf("\"%s\"", string->ptr);
}

void string_printNewLine(string_t* string){
   printf("\"%s\"\n", string->ptr);
}

int* string_isSubstring(string_t* string, string_t* sub){
   int aux, aux1, size = 0;
   int* resp = NULL;
   resp = (int*) malloc( sizeof(int) );
   for(aux = 0; aux < string->size; aux ++){
      for(aux1 = 0; string->ptr[aux + aux1] == sub->ptr[aux1] && string->ptr[aux + aux1] != '\0'; aux1 ++);
      if(sub->ptr[aux1] == '\0' && string->ptr[aux + aux1] != '\0'){
         resp = (int*) realloc(resp, (++size + 1) * sizeof(int));
         resp[size - 1] = aux;
      }
   }
   resp[size] = -1;
   return resp;
}

int* string_isSubstringLiteral(string_t* string, char* sub){
   int aux, aux1, size = 0;
   int* resp = NULL;
   resp = (int*) malloc( sizeof(int) );
   for(aux = 0; aux < string->size; aux ++){
      for(aux1 = 0; string->ptr[aux + aux1] == sub[aux1] && string->ptr[aux + aux1] != '\0'; aux1 ++);
      if(sub[aux1] == '\0'){
         resp = (int*) realloc(resp, (++size + 1) * sizeof(int));
         resp[size - 1] = aux;
      }
   }
   resp[size] = -1;
   return resp;
}

void string_reverse(string_t* string){
   int aux;
   for(aux = 0;aux < string->size/2; aux ++){
      char buffer = string->ptr[aux];
      string->ptr[aux] = string->ptr[string->size - aux - 1];
      string->ptr[string->size - aux - 1] = buffer;
   }
}

string_t* string_getSubstring(string_t* string, int start, int end){
   string_t* sub = string_create(NULL);
   int add = 1, aux;
   if(string->size == 0) return string_create(NULL);
   while(start < 0) start += string->size;
   while(start >= string->size) start -= string->size;
   while(end < 0) end += string->size;
   while(end >= string->size) end -= string->size;

   if(start > end){
      add = -1;
   }

   string_realloc(sub, (end - start) * add + 2);
   sub->size--;

   for(aux = start; aux - add != end; aux += add){
      sub->ptr[(aux - start) * add] = string->ptr[aux];
   }
   sub->ptr[(aux - start) * add] = '\0';
   sub->ptr[(aux - start) * add + 1] = 'a';
   return sub;
}

string_t* string_readFileLine(FILE* data){
   string_t* line = string_create(NULL);
   do{
      string_realloc(line, line->size + 1);
      line->ptr[line->size - 1] = fgetc(data);
   } while(line->ptr[line->size - 1] != '\n' && !feof(data));
   line->ptr[--line->size] = '\0';
   return line;
}

string_t* string_readFileWord(FILE* data){
   string_t* word = string_create(NULL);
   do{
      string_realloc(word, word->size + 1);
      word->ptr[word->size - 1] = fgetc(data);
   } while(
      ((word->ptr[word->size - 1] >= 'a' && 
      word->ptr[word->size - 1] <= 'z') ||
      (word->ptr[word->size - 1] >= 'A' && 
      word->ptr[word->size - 1] >= 'Z') ||
      word->ptr[word->size - 1] == '_'||
      word->ptr[word->size - 1] == '-') &&
      !feof(data)
   );
   string_realloc(word, word->size + 1);
   word->ptr[--word->size] = '\0';
//   fseek(data, -1, SEEK_CUR);
   return word;
}

string_t* string_fileGetTokens(FILE* data){
   string_t* word = string_create(NULL);
   do{
      string_realloc(word, word->size + 1);
      word->ptr[word->size - 1] = fgetc(data);
   } while(
      ((word->ptr[word->size - 1] >= 'a' && 
      word->ptr[word->size - 1] <= 'z') ||
      (word->ptr[word->size - 1] >= 'A' && 
      word->ptr[word->size - 1] >= 'Z') ||
      word->ptr[word->size - 1] == '_'||
      word->ptr[word->size - 1] == '-') &&
      !feof(data)
   );
   string_realloc(word, word->size + 1);
   word->ptr[--word->size] = '\0';
//   fseek(data, -1, SEEK_CUR);
   return word;
}

int string_sizeofLiteral(char* string){
   int aux;
   for(aux = 0;string[aux] != '\0'; aux ++);
   return aux;
}

string_t* string_scanfWord(){
   string_t* word = string_create(NULL);
   do{
      string_realloc(word, word->size + 1);
      scanf("%c",&(word->ptr[word->size - 1]));
   } while(
      (word->ptr[word->size - 1] >= 'a' && 
      word->ptr[word->size - 1] <= 'z') ||
      (word->ptr[word->size - 1] >= 'A' && 
      word->ptr[word->size - 1] >= 'Z') ||
      word->ptr[word->size - 1] == '_'||
      word->ptr[word->size - 1] == '-'
   );
   word->ptr[--word->size] = '\0';
   return word;
}

string_t* string_scanfLine(){
   string_t* line = string_create(NULL);
   do{
      string_addSize(line, 1);
      scanf("%c", &(line->ptr[line->size - 1]));
   } while(line->ptr[line->size - 1] != '\n');
   line->ptr[--line->size] = '\0';
   return line;
}

void string_trim(string_t* string){
   int aux, aux1, aux2;
   if(string->size == 0) return;
   for(aux = 0;
      (string->ptr[aux] == ' ' ||
      string->ptr[aux] == '\t' ||
      string->ptr[aux] == '\n') &&
      string->ptr[aux] != '\0';
   aux ++);

   for(aux1 = string->size - 1;
      aux1 >= 0 &&
      (string->ptr[aux1] == ' ' || 
      string->ptr[aux1] == '\t' ||
      string->ptr[aux1] == '\n' ||
      string->ptr[aux1] == '\0');
   aux1 --);

   aux1++;

   for(aux2 = 0; aux + aux2 < aux1; aux2 ++){
      string->ptr[aux2] = string->ptr[aux + aux2];
   }
   string->ptr[aux2] = '\0';
   string->size = aux2;
}

void string_trimLeft(string_t* string){
   int aux, aux1;
   for(aux = 0;
      (string->ptr[aux] == ' ' ||
      string->ptr[aux] == '\t' ||
      string->ptr[aux] == '\n') &&
      string->ptr[aux] != '\0';
   aux ++);
   
   for(aux1 = 0; aux1 + aux < string->size; aux1 ++){
      string->ptr[aux1] = string->ptr[aux1 + aux];
   }
   string->ptr[aux1] = '\0';
   string->size = aux1;
}

void string_removeCharacters(string_t* string, char* chars){
   int aux, aux1;
   int offset = 0;
   for(aux = 0; aux + offset< string->size; aux ++){
      for(aux1 = 0;chars[aux1] != '\0' && aux + offset < string->size; aux1 ++){
         if(string->ptr[aux + offset] == chars[aux1]){
            offset ++;
            aux1 = -1;
         }
      }
      string->ptr[aux] = string->ptr[aux + offset];
   }
}

string_t** string_split(string_t* string, char* splitTokens){
   string_t** answer = NULL;
   int size = 0, aux, lastOccurrence = 0;
   int* occurrences;
   int splitTokensSize;

   answer = (string_t**) malloc(sizeof(string_t*));

   splitTokensSize = string_sizeofLiteral(splitTokens);
   occurrences = string_isSubstringLiteral(string, splitTokens);

   for(aux = 0;occurrences[aux] != -1; aux ++){
      answer = (string_t**) realloc(answer, sizeof(string_t*) * (size + 3));
      answer[size++] = string_getSubstring(string, lastOccurrence, occurrences[aux] - 1);
      lastOccurrence = occurrences[aux] + splitTokensSize;
   }
   answer[size++] = string_getSubstring(string, lastOccurrence, -1);
   answer[size] = NULL;
   free(occurrences);
   return answer;
}

string_t* string_flush(string_t** strings){
   int aux;
   string_t* result = string_duplicate(strings[0]);
   for(aux = 1;strings[aux] != NULL; aux++){
      string_concatenate(result, strings[aux]);
   }
   return result;
}

void string_printAscii(string_t* string){
   int aux;
   printf("%d", string->ptr[0]);
   for(aux = 1; aux < string->size; aux ++){
      printf(", %d", (int)string->ptr[aux]);
   }
   printf("\n");
}

int string_findChar(string_t* string, char c){
   int aux;
   for(aux = 0;aux < string->size; aux ++){
      if(string->ptr[aux] == c)
         return aux;
   }
   return -1;
}

//returns the position of the first char found from tokens found in string
int string_findChars(string_t* string, char* tokens){
   int aux, aux1;
   for(aux = 0;aux < string->size; aux ++){
      for(aux1 = 0;tokens[aux1] != '\0'; aux1 ++){
         if(string->ptr[aux] == tokens[aux1]) return aux;
      }
   }
   return -1;
}

//returns true if all tokens are present
int string_findAllChars(string_t* string, char* tokens){
   int aux, aux1;
   for(aux = 0;tokens[aux] != '\0'; aux ++){
      char flag = 0;
      for(aux1 = 0; aux1 < string->size; aux1 ++){
         if(string->ptr[aux1] == tokens[aux]){
            flag = 1;
            break;
         }
      }
      if(flag == 0) return 0;
   }
   return 1;
}

void string_crop(string_t* string, int point){
   if(point < 0 || point > string->size){
      printf("something went wrong in the string crop function\n");
      return;
   }
   string->size = point;
   string->ptr[point] = '\0';
}

void string_toUpper(string_t* string){
   int aux;
   for(aux = 0;aux < string->size; aux ++){
      if(string->ptr[aux] >= 'a' && string->ptr[aux] <= 'z')
         string->ptr[aux] += 'A' - 'a';
   }
}

void string_toLower(string_t* string){
   int aux;
   for(aux = 0;aux < string->size; aux ++){
      if(string->ptr[aux] >= 'A' && string->ptr[aux] <= 'Z')
         string->ptr[aux] -= 'A' - 'a';
   }
}
