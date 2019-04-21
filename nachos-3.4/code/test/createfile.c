#include "syscall.h"

int main()
{
    OpenFileId stdin = ConsoleInput;
    OpenFileId stdout = ConsoleOutput;
    char buffer[256];

    Write("Enter filename: ", 256, stdout);
    Read(buffer, 256, stdin);

    if (Create(buffer) == -1)
    {
        Write("Unable to create file ", 256, stdout);
        Write(buffer, 256, stdout);
        Write("\n", 1, stdout);
    }

    return 0;
}