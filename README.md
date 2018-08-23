# Maker

This is a project designed to automate the cration of Makefile and header files in **C** programs.

given a main file, containing the main function, the maker will automatically generate missing header files and generate the Makefile. It will also display written lines of code, for convinience.

# Limitations

when including files, you must use quotation marks includes instead of the angled quotes one. Use
` #include"myHeader.h" `
instead of
` #include<myHeader.h> `

File headers must have the same exact name as source code files, even if the headers are missing, they must be included in the correct name.  **myCode.c** will have a **myCode.h** file, that will be included as ` #include"myCode.h" `, even if the header is missing.

relative path is not allowed, since no support was added for that.

# behaviour

To run Maker, simply type ` Maker mainFile.c `. all source files will be recursively opened, checked for new header files, and closed, starting with mainFile.c. After all relevant source codes are read, missing header files will be detected and generated. then a Makefile will be created, considering all headers and source codes. The Makefile will have a **run**, **all**, **valgrind** and **zip** tags:

- all: just compile the code.
- run: compile and run the code.
- valgrind: compile and run using valgrind.
- zip: zip all header and source files.

When compiling code, all object files will be stored in a folder named "objectFiles".
