#include "syscall.h"

int main()
{
    OpenFileId stdout = ConsoleOutput;
    int i = 0;
    for (;i < 1000; ++i)
    {
        Write("B", 1, stdout);
    }

    return 2;
}