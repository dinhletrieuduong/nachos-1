#include "syscall.h"
#include "copyright.h"

int main()
{
    char fileName[256];
    char c;
    int fileSize;
    int i;
    int len;
    OpenFileId file;
    
    PrintString("Enter file name: ");
    len = ReadString(fileName);
    
    if (len > 0) 
    {
        file = OpenFileID(fileName, 1);
        
        if (file > 0) 
        {
            fileSize = SeekFile(-1, file);
            i = 0;
            SeekFile(0, file);
            
            for (; i < fileSize; ++i)
            {
                ReadFile(&c, 1, file);
                PrintChar(c);
            }
        }
    }
    
    Halt();
    return 0;
}  
