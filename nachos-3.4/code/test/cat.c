#include "syscall.h"
#include "copyright.h"

/*int ReadLine(char* buffer, OpenFileId)
{
    int i = 0;
    do {
        Read(&buffer[i], 1, OpenFileId);
    } while (buffer[i] == '\n');

    buffer[--i] = '\0'

    return i;
}

int main()
{
    SpaceId newProc;
    char buffer[60];
    int i = 0;

    ReadLine(buffer, ConsoleInput);

    OpenFileId file = OpenFileID(buffer, 1);

    if (file == -1)
    {
        PrintfString("File not found");
        return 1;
    }

    while (ReadLine(buffer, file) < 0)
    {

    }

    return 0;
}*/
int main()
{
    return 0;
}  
