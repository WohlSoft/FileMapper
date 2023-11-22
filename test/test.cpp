#include <stdio.h>
#include <string>
#include <FileMapper/file_mapper.h>

int main(int , char**)
{
    FileMapper map("TestFile.txt");
    if(!map.data())
    {
        printf("Can't open file!\n");
        return 1;
    }

    std::string in(reinterpret_cast<char*>(map.data()), map.size());
    printf("Got data:\n%s\n\nTest done!", in.c_str());

    return 0;
}
