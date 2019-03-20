#include "syscall.h"
#include "copyright.h"

int main()
{
    char buffer[256];
    int len = ReadLine(buffer, ConsoleInput);
    Halt();
    return 0;
}  
