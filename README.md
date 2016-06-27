# File Mapper

A small C++ class which providing a cross-platform read-only file mapping interface.

# About file mapping

File mapping allows you to read/write file data like it's a memory segment. In result is a
faster performance in comparison with regular file I/O APIs and also allows to use less memory
while working with a huge files. Also possible to use a files as memory banks to optimize
a memory usage.

# Example of usage
```C++
#include "file_mapper.h"

//...

FileMapper myfile;

if( !myfile.open("/path/to/the/hugetankofoil.dat") ) //All paths are must be encoded in UTF-8
{
    printf( "CAN'T OPEN FILE BECAUSE %s", myfile.error().c_str() );
    return false;
}

//Pointer to the file data
char* data = myfile.data;

//Size of the file in bytes
unsigned long filesize = myfile.size;

/*
Then you can manipulate with file data like it's data in the memory.
*/

//When you done work with a file
myfile.close_file();

//But you just can keep constructed class be opened if you need a presistant access to the file data

//...

```