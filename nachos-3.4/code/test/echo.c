#include "syscall.h"
#include "copyright.h"

int main()
{
    char buffer[256];
    int bufCount;
    
    while (1)
    {
        PrintString("Read: ");
        bufCount = ReadString(buffer);
        
        if (bufCount == 1 && buffer[0] == 'e')
        {
            PrintString("Exit command detected!\n");
            break;
        }
        
        PrintString("Echo: ");
        PrintString(buffer);
        PrintString("\n");
    }
    
    Halt();
    return 0;
}  
