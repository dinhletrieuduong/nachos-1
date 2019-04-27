#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "pcb.h"

#define MAX_FILE_LENGTH 256

void SCF_Exit()
{
    printf("\nExit %d!\n", machine->ReadRegister(4));
}

int SCF_Exec()
{
    int addr = machine->ReadRegister(4);
    char* filename = machine->User2System(addr, MAX_FILE_LENGTH);
    int id = gPTable->ExecUpdate(filename);
    delete filename;
    return id;
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
    char buffer[numBytes];
    
    int count = gFTable->Read(buffer, numBytes, fileid);

    machine->System2User(addr, buffer, numBytes);

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

int SCF_Int2Str()
{
    int addr = machine->ReadRegister(4);
    int dec = machine->ReadRegister(5);
    char buff[1024];
    sprintf(buff, "%d", dec);
    machine->System2User(addr, buff, 1024);
    return addr;
}

int SCF_Str2Int()
{
    int addr = machine->ReadRegister(4);
    char* buff = machine->User2System(addr, 1024);
    //printf("%d\n", atoi(buff));
    return atoi(buff);
}

void SCF_Writeln()
{
    int addr = machine->ReadRegister(4);
    char* buffer = machine->User2System(addr, 1024);
    gFTable->Write(buffer, strlen(buffer), 1);
    gFTable->Write("\n", 1, 1);
}

// @FIXME
int SCF_Readln()
{
    int addr = machine->ReadRegister(4);
    char buffer[1024];
    
    int count = gFTable->Read(buffer, 1024, 0);

    machine->System2User(addr, buffer, 1024);

    return count;
}

