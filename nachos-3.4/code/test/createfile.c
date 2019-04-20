#include "syscall.h"

int main()
{
    OpenFileId stdin = ConsoleInput;
    OpenFileId stdout = ConsoleOutput;
    OpenFileId ourfile;
    char buffer[256];
    char c;
    int len = 0;
    int i = 0;
    
    Write("Enter file name: ", 256, stdout);
    Read(buffer, 256, stdin);
    Write("Open file: ", 256, stdout);
    Write(buffer, 256, stdout);
    Write("\n", 1, stdout);

    ourfile = Open(buffer, 1);

    if (ourfile == -1)
    {
        Write("Error while opening file\n", 256, stdout);
        return 1;
    }

    Write("Data: ", 100, stdout);
    
    len = Seek(-1, ourfile);

    for (;i < len; ++i)
    {
        Read(&c, 1, ourfile);
        Write(&c, 1, stdout);
    }
    Write("\n", 1, stdout);

    return 0;
}