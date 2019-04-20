#include "copyright.h"
#include "system.h"
#include "syscall.h"

#define MAX_FILE_LENGTH 32

void SCF_Exit()
{

}

int SCF_Create()
{
    int addr = machine->ReadRegister(4);
    char* filename = machine->User2System(addr, MAX_FILE_LENGTH);

    if (filename == NULL)
    {
        printf("Not enough memory!\n");
        delete filename;
        return -1;
    }

    if (!fileSystem->Create(filename, 0))
    {
        printf("Error while creating file!\n");
        delete filename;
        return -1;
    }

    delete filename;
    return 0;
}

int SCF_Open()
{
    int addr = machine->ReadRegister(4);
    int type = machine->ReadRegister(5);
    char* filename = machine->User2System(addr, MAX_FILE_LENGTH);

    if (filename == NULL)
    {
        printf("Not enough memory\n");
        delete filename;
        return -1;
    }

    int index = gFTable->Open(filename, type);
    delete filename;
    return index;
}

int SCF_Read()
{
    int addr = machine->ReadRegister(4);
    int numBytes = machine->ReadRegister(5);
    int fileid = machine->ReadRegister(6);
    char *buffer = new char[numBytes];
    
    int count = gFTable->Read(buffer, numBytes, fileid);

    machine->System2User(addr, buffer, numBytes);

    //printf("Read: %s\n", buffer);

    return count;
}

int SCF_Seek()
{
    int pos = machine->ReadRegister(4);
    int fileid = machine->ReadRegister(5);
    // printf("Pos: %d\n", pos);
    // printf("Fileid: %d\n", fileid);
    return gFTable->Seek(pos, fileid);
}

void SCF_Write()
{
    // printf("Write!\n");
    int addr = machine->ReadRegister(4);
    int numBytes = machine->ReadRegister(5);
    int fileid = machine->ReadRegister(6);
    char* buffer = machine->User2System(addr, numBytes);

    gFTable->Write(buffer, strlen(buffer), fileid);
    //printf("Write: %s\n", buffer);
}

void SCF_Close()
{
    int fileid = machine->ReadRegister(4);
    gFTable->Close(fileid);
}