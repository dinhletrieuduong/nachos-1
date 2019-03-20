#include "syscall.h"
#include "copyright.h"

int main()
{
    char source[256];
    char dest[256];
    char temp;
    int fileSize;
    int count;
    OpenFileId sourceFile;
    OpenFileId destFile;
    
    PrintString("Enter source: ");
    ReadString(source);
    PrintString("Enter destination: ");
    ReadString(dest);
    
    CreateFile(dest);
    
    sourceFile = OpenFileID(source, 1);
    destFile = OpenFileID(dest, 0);
    
    if (sourceFile < 0)
    {
        PrintString("Failed to open ");
        PrintString(source);
        return 1;
    }
    
    fileSize = SeekFile(-1, sourceFile);
    count = 0;
    SeekFile(0, sourceFile);
    
    for (; count < fileSize; ++count)   
    {
        ReadFile(&temp, 1, sourceFile);
        WriteFile(&temp, 1, destFile);
    }
    
    return 0;
}
