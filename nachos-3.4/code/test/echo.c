#include "syscall.h"

int main()
{
    OpenFileId stdin = ConsoleInput;
    OpenFileId stdout = ConsoleOutput;
    char buffer[1024];

    while (1)
    {
        Write("Enter: ", 256, stdout);
        Read(buffer, 1024, stdin);
        Write("Echo: ", 256, stdout);
        Write(buffer, 1024, stdout);
        Write("\n", 1, stdout);
    }
    return 0;
}