#include "syscallfuncs.h"

void SCF_CreateFile()
{
    static const int MaxFileLength = 32;
    int virtAddr = machine->ReadRegister(4);
    char* fileName = machine->User2System(virtAddr, MaxFileLength + 1);
    
    if (fileName == NULL)
    {
        printf("Not enough memory in system\n");
        machine->WriteRegister(2, -1);
        delete fileName;
        return;
    }
    
    printf("Create new file \'%s\'\n", fileName);
    
    if (!fileSystem->Create(fileName, 0))
    {
        printf("Error while creating file \'%s\'\n", fileName);
        machine->WriteRegister(2, -1);
        delete fileName;
        return;
    }
    
    machine->WriteRegister(2, 0);
    delete fileName;
}