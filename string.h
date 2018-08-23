#ifndef _LIBRARY_STRING
#define _LIBRARY_STRING


typedef struct string_t{
   char* ptr;
   char* base;
   int size;
} string_t;

string_t* string_create(char* arg);
void string_delete(string_t* string);
string_t* string_duplicate(string_t* string);
void string_realloc(string_t* string, size_t size);
void string_addSize(string_t* string, int size);
void string_concatenateLiteral(string_t* a, char* c);
void string_concatenate(string_t* a, string_t* b);
void string_shift(string_t* string, int offset);
void string_unshift(string_t* string);
int string_compare(string_t* string1, string_t* string2);
int string_compareLiteral(string_t* string1, char* string2);
int string_checkIntegrity(string_t* string);
void string_print(string_t* string);
void string_printNewLine(string_t* string);
int* string_isSubstring(string_t* string, string_t* sub);
int* string_isSubstringLiteral(string_t* string, char* sub);
void string_reverse(string_t* string);
string_t* string_getSubstring(string_t* string, int start, int end);
string_t* string_readFileLine(FILE* data);
string_t* string_readFileWord(FILE* data);
string_t* string_fileGetTokens(FILE* data);
int string_sizeofLiteral(char* string);
string_t* string_scanfWord();
string_t* string_scanfLine();
void string_trim(string_t* string);
void string_trimLeft(string_t* string);
void string_removeCharacters(string_t* string, char* chars);
string_t** string_split(string_t* string, char* splitTokens);
string_t* string_flush(string_t** strings);
void string_printAscii(string_t* string);
int string_findChar(string_t* string, char c);
int string_findChars(string_t* string, char* tokens);
int string_findAllChars(string_t* string, char* tokens);
void string_crop(string_t* string, int point);
void string_toUpper(string_t* string);
void string_toLower(string_t* string);
#endif
