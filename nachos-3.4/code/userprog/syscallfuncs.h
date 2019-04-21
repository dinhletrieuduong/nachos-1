#include "copyright.h"
#include "system.h"
#include "syscall.h"

#define MAX_FILE_LENGTH 256

void __StartProcess(int addr)
{
    printf("Address: %d\n", addr);
    char* filename = machine->User2System(addr, MAX_FILE_LENGTH);
    OpenFile *executable = fileSystem->Open(filename);
    AddrSpace *space;

    printf("Execute %s\n", filename);

    if (executable == NULL) {
	    printf("Unable to open file %s\n", filename);
        delete filename;
	    ASSERT(FALSE);
    }

    delete filename;

    space = new AddrSpace(executable);    
    currentThread->space = space;

    delete executable;			// close file

    space->InitRegisters();		// set the initial register values
    space->RestoreState();		// load page table register

    machine->Run();			// jump to the user progam
    ASSERT(FALSE);			// machine->Run never returns;
					// the address space exits
					// by doing the syscall "exit"
}

void SCF_Exit()
{
}

int SCF_Exec()
{
    static int id = 0;
    char sid[3];
    int addr = machine->ReadRegister(4);

    sid[0] = (id / 10) + '0';
    sid[1] = (id % 10) + '0';
    sid[2] = 0;

    __StartProcess(addr);

    return id++;
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