#include "syscall.h"

int main()
{
    OpenFileId stdout = ConsoleOutput;
    int i = 0;
    for (;i < 1000; ++i)
    {
        Write("A", 1, stdout);
    }
}