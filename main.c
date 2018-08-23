#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"vector.h"
#include"string.h"
#include"list.h"

typedef struct tokens{
   char remakeAllHeaders;
} tokens_t;

tokens_t* tokens;

typedef struct file_t{
   string_t* name;
   string_t* headerName;
   string_t* objectName;
   string_t* extensionlessName;
   vector_t* deps;
   int lines;
   int headerLines;
} file_t;

tokens_t* tokens_create(){
   return calloc(1, sizeof(tokens_t));
}

file_t* file_create(){
   file_t* file = calloc(1, sizeof(file_t));
   file->deps = vector_create(sizeof(file_t*));
   return file;
}

void string_capExtension(string_t* line){
   int aux;
   for(aux = line->size - 1; aux >= 0; aux --){
      if(line->ptr[aux] == '.')
         string_crop(line,aux);
   }
}

void gotoFileLocation(string_t* fileName){
   int aux;
   for(aux = fileName->size - 1;aux >= 0 && fileName->ptr[aux] != '/'; aux --);
   if(aux == -1) return;
   fileName->ptr[aux] = '\0';
   if(chdir(fileName->ptr) != 0){
      printf("Could not locate folder goto \"%s\"\n",fileName->ptr);
      exit(-1);
   }
   fileName->ptr[aux] = '/';
   string_shift(fileName, aux + 1);
}

void file_initializeNames(file_t* file){
   if(file->name != NULL){
      file->extensionlessName = string_duplicate(file->name);
      string_delete(file->name);
   }
   if(file->headerName != NULL){
      file->extensionlessName = string_duplicate(file->headerName);
      string_delete(file->headerName);
   }
   if(file->objectName != NULL){
      file->extensionlessName = string_duplicate(file->objectName);
      string_delete(file->objectName);
   }
   string_capExtension(file->extensionlessName);
   file->name = string_duplicate(file->extensionlessName);
   string_concatenateLiteral(file->name, ".c");
   file->headerName = string_duplicate(file->extensionlessName);
   string_concatenateLiteral(file->headerName, ".h");
   file->objectName = string_duplicate(file->extensionlessName);
   string_concatenateLiteral(file->objectName, ".o");
}

void file_delete(file_t* file){
   int aux;
   for(aux = 0;aux < file->deps->size; aux ++){
      string_delete(((string_t*)(file->deps->data[aux])));
   }
   vector_delete(file->deps);
   string_delete(file->name);
   string_delete(file->headerName);
   string_delete(file->objectName);
   string_delete(file->extensionlessName);
   free(file);
}

FILE* openFile(char* name, char* mode){
   FILE* file = fopen(name, mode);
   if(file == NULL){
      printf("Could not open file \"%s\" for ", name);
      if(mode[0] == 'w') printf("writing\n");
      else printf("reading\n");
      exit(-1);
   }
   return file;
}

void getFileDependencies(file_t* targetFile){
   FILE* file = openFile(targetFile->name->ptr, "r");
   targetFile->lines = 0;

   while(!feof(file)){
      string_t* line = string_readFileLine(file);
      targetFile->lines += 1;
      string_removeCharacters(line, " \t\n");
      int* substringPos = string_isSubstringLiteral(line, "#include\"");
      if(substringPos[0] == 0){
         int aux;
         file_t* newDep = file_create();
         string_shift(line, substringPos[0] + 9);
         for(aux = 0; line->ptr[aux] != '"' && aux < line->size; aux ++);
         string_crop(line, aux);
         newDep->headerName = string_duplicate(line);
         file_initializeNames(newDep);

         vector_push(targetFile->deps, newDep);
      }
      free(substringPos);
      string_delete(line);
   }
   targetFile->lines --;
}

void readAllFiles_recursion(file_t* mainFile, vector_t* files){
   int aux;
   for(aux = 0;aux < files->size; aux ++){
      if(string_compare(mainFile->name, ((file_t*)(files->data[aux]))->name) == -1)
         return;
   }
   vector_push(files, mainFile);
   for(aux = 0; aux < mainFile->deps->size; aux++){
      file_t* newFile = (file_t*)(mainFile->deps->data[aux]);

      getFileDependencies(newFile);
      readAllFiles_recursion(newFile, files);
   }
}

vector_t* readAllFiles(string_t* mainName){
   vector_t* files = vector_create(sizeof(file_t*));
   file_t* mainFile = file_create();

   mainFile->name = mainName;
   file_initializeNames(mainFile);

   getFileDependencies(mainFile);
   readAllFiles_recursion(mainFile, files);
   return files;
}

void string_handleQuotesAndComments(string_t* line){
   int aux;
   for(aux = 0;aux < line->size; aux ++){
      if(line->ptr[aux] == '\''){
         if(line->ptr[aux + 1] == '\\'){
            line->ptr[aux + 1] = 'X';
            line->ptr[aux + 2] = 'X';
            aux += 3;
         }
         else if(line->ptr[aux + 1] == '\'')
            aux += 2;
         else if(line->ptr[aux + 1] == '\0')
            aux ++;
         else{
            line->ptr[aux + 1] = 'X';
            aux += 2;
         }
      }
      else if(line->ptr[aux] == '"'){
         aux ++;
         while(line->ptr[aux] != '"'){
            if(line->ptr[aux] == '\\' && line->ptr[aux + 1] != '\0')
               line->ptr[aux + 1] = 'X';
            line->ptr[aux] = 'X';
            aux ++;
         }
      }
      else if(line->ptr[aux] == '/' && line->ptr[aux + 1] == '/'){
         line->ptr[aux] = '\0';
         line->size = aux;
      }
   }
}

void createHeader(file_t* file){
   int nestLevel = 0;
   int aux;
   FILE* read = openFile(file->name->ptr, "r");
   FILE* write = openFile(file->headerName->ptr, "w");
   string_t* nameUpper = string_create(file->extensionlessName->ptr);
   string_toUpper(nameUpper);
   fprintf(write, "#ifndef _LIBRARY_%s\n#define _LIBRARY_%s\n\n", nameUpper->ptr, nameUpper->ptr);
   file->headerLines += 3;
   free(nameUpper);
   string_t* line;

   //write all the includes of the file
   for(aux = 0;aux < file->deps->size; aux ++){
      string_t* headerName = ((file_t*)(file->deps->data[aux]))->headerName;
      fprintf(write, "#include \"%s\"\n", headerName->ptr);
      file->headerLines++;
   }
   fprintf(write, "\n");
   file->headerLines++;

   while(!feof(read)){
      line = string_readFileLine(read);
      string_trim(line);
      string_handleQuotesAndComments(line);
      int* subPos = string_isSubstringLiteral(line, "typedef");
      if(subPos[0] != -1){
         fprintf(write, "%s\n", line->ptr);
         file->headerLines++;
         if(line->ptr[line->size - 1] == '{') do{
            line = string_readFileLine(read);
            fprintf(write, "%s\n", line->ptr);
            file->headerLines++;
            string_trim(line);
            string_handleQuotesAndComments(line);
         } while(string_findChar(line, '}') == -1 && !feof(read));
         fprintf(write, "\n");
         file->headerLines++;
         line = string_readFileLine(read);
         string_trim(line);
         string_handleQuotesAndComments(line);
      }
      free(subPos);
      if(string_findChar(line, '{') != -1){
         if(nestLevel == 0 && string_findAllChars(line, "()")){
            line->ptr[line->size - 1] = ';';
            fprintf(write, "%s\n", line->ptr);
            file->headerLines++;
         }
         nestLevel ++;
      }
      if(string_findChar(line, '}') != -1){
         nestLevel --;
      }
   }
   fprintf(write, "#endif\n");
   file->headerLines++;
}

void printStringVector(vector_t* vec){
   int aux;
   for(aux = 0; aux < vec->size; aux ++){
      string_printNewLine(((file_t*)(vec->data[aux]))->name);
   }
}

void printAllFiles(vector_t* files){
   int aux;
   for(aux = 0;aux < files->size; aux ++){
      printf("From file \"%s\":\n", ((file_t*)(files->data[aux]))->name->ptr);
      int aux1;
      for(aux1 = 0; aux1 < ((file_t*)(files->data[aux]))->deps->size; aux1 ++){
         printf("\tdep \"%s\"\n", ((string_t*)(((file_t*)(files->data[aux]))->deps->data[aux1]))->ptr);
      }
   }
}

string_t* interpretArguments(int argc, char** argv){
   int aux;

   string_t* mainFileName = NULL;

   for(aux = 1;aux < argc; aux ++){
      if(argv[aux][0] == '-'){
         if(argv[aux][1] == 'u'){
            if(aux + 1 >= argc){
               printf("expected update arguments\n");
            }
            if(argv[aux + 1][0] == '.' && argv[aux + 1][1] == '\0')
               tokens->remakeAllHeaders = 1;
            else{
               file_t* updateFile = file_create();
               updateFile->name = string_create(argv[aux + 1]);
               file_initializeNames(updateFile);
               createHeader(updateFile);
               printf("forcing creation of header for \"%s\"...\n", updateFile->name->ptr);
               file_delete(updateFile);
            }
            aux ++;
         }
      }
      else if(mainFileName != NULL){
         printf("You may only specify one main file\n");
         exit(-1);
      }
      else{
         mainFileName = string_create(argv[aux]);
         gotoFileLocation(mainFileName);
      }
   }
   if(mainFileName == NULL || mainFileName->size == 0){
      printf("Main file name not specified, finishing program\n");
      exit(0);
   }
   return mainFileName;
}

void generateMissingHeadersAndCountHeaderLines(vector_t* files){
   int aux, aux1;
   for(aux = 1;aux < files->size; aux ++){
      file_t* currentFile = (file_t*)(files->data[aux]);
      string_t* newFileName = currentFile->headerName;
      FILE* header = fopen(newFileName->ptr, "r");
      if(header == NULL){
         printf("header \"%s\" not found, generating new header...\n", newFileName->ptr);
         createHeader(currentFile);
      }
      else{
         currentFile->headerLines = 0;
         while(!feof(header)){
            char c = fgetc(header);
            if(c == '\n') currentFile->headerLines++;
         }
         fclose(header);
      }
   }
}

void generateAllHeaders(vector_t* files){
   int aux;
   for(aux = 1; aux < files->size; aux ++){
      file_t* file = (file_t*)(files->data[aux]);
      printf("forcing generation of header \"%s\"\n", file->headerName->ptr);
      createHeader(file);
   }
}

void writeMakefile(vector_t* files){
   FILE* write = openFile("Makefile", "w");
   fprintf(write, "objectsFolder = objectFiles\n\n");
   fprintf(write, "exe: all\n\t./exe\n\nall:");
   fprintf(write, " createDir start\n\ncreateDir:\n\t@if [ ! -d \"$(objectsFolder)\" ]; then mkdir $(objectsFolder);fi\n\nstart:");
   int aux;
   for(aux = 0; aux < files->size; aux ++){
      string_t* objectName = ((file_t*)(files->data[aux]))->objectName;
      fprintf(write, " $(objectsFolder)/%s", objectName->ptr);
   }
   fprintf(write, "\n\tgcc -o exe");
   for(aux = 0;aux < files->size; aux ++){
      string_t* objectName = ((file_t*)(files->data[aux]))->objectName;
      fprintf(write, " $(objectsFolder)/%s", objectName->ptr);
   }
   fprintf(write, "\n\n");
   for(aux = 0; aux < files->size; aux ++){
      char* objectName = ((file_t*)(files->data[aux]))->objectName->ptr;
      char* fileName = ((file_t*)(files->data[aux]))->name->ptr;
      fprintf(write, "$(objectsFolder)/%s: %s\n\tgcc -c %s -o $(objectsFolder)/%s\n\n", objectName, fileName, fileName, objectName);
   }
   fprintf(write, "zip: all\n\tzip main.zip %s", ((file_t*)(files->data[0]))->name->ptr);
   for(aux = 1;aux < files->size; aux ++){
      char* name = ((file_t*)(files->data[aux]))->name->ptr;
      char* headerName = ((file_t*)(files->data[aux]))->headerName->ptr;
      fprintf(write, " %s %s", name, headerName);
   }
   fprintf(write, "\n\nvalgrind: all\n\tvalgrind ./exe");
}

void sayTotalLines(vector_t* files){
   int aux, totalLines = 0, totalHeaderLines = 0;
   printf("\n");
   for(aux = 0; aux < files->size; aux ++){
      file_t* file = (file_t*)(files->data[aux]);
      printf("found %d lines for file \"%s\" and %d for its header\n", file->lines, file->name->ptr, file->headerLines);
      totalLines += file->lines;
      totalHeaderLines += file->headerLines;
   }
   printf("\ntotal writen lines: %d\ntotal header lines: %d\ntotal lines: %d\n", totalLines, totalHeaderLines, totalLines + totalHeaderLines);
}

int main(int argc, char** argv){
   tokens = tokens_create();
   string_t* mainFileName = interpretArguments(argc, argv);
   vector_t* files = readAllFiles(mainFileName);
   if(tokens->remakeAllHeaders == 0)
      generateMissingHeadersAndCountHeaderLines(files);
   else
      generateAllHeaders(files);
   sayTotalLines(files);
   writeMakefile(files);
}
