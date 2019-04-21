#include "syscall.h"

int main()
{
    OpenFileId stdin = ConsoleInput;
    OpenFileId stdout = ConsoleOutput;
    OpenFileId ourfile = -1;
    char buffer[256];
    int filesize;
    int i;

    Write("Enter filename: ", 256, stdout);
    Read(buffer, 256, stdin);

    ourfile = Open(buffer, 1);

    if (ourfile == -1)
    {
        Write("Failed to open file\n", 256, stdout);
        return 1;
    }

    filesize = Seek(-1, ourfile);

    for (i = 0; i < filesize;)
    {
        Read(buffer, 256, ourfile);
        Write(buffer, 256, stdout);
        i += 256;
    }

    return 0;
}