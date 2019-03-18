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
int SCF_Read(){
	int buffer = machine->ReadRegister(4);
	int charCount = machine->ReadRegister(5);
	int id = machine->ReadRegister(6);	
	int OldPos;
	int NewPos;
	char *buf = new char[charCount];
	int i = 0;
	// Check id
	if (id < 0 || id > 10)
	{
		machine->WriteRegister(2, -1);
		delete[] buf;
		return -1;
	}
	// check openf[id]
	if (fileSystem->openf[id] == NULL)
	{
		machine->WriteRegister(2, -1);
		delete[] buf;
		return -1;
	}
	OldPos = fileSystem->openf[id]->GetCurrentPos();
	buf = machine->User2System(buffer, charCount);
	if (fileSystem->openf[id]->type == 2)
	{
		/*  printf("charCount = %d\n", charCount);*/
		int sz = gSynchConsole->Read(buf, charCount);
		/*  machine->System2User(buffer, sz, buf);*/
	
		machine->System2User(buffer, sz, buf);
		machine->WriteRegister(2, sz);
		return sz;
	}
	
	if ((fileSystem->openf[id]->Read(buf, charCount) ) > 0)
	{
		// Copy data from kernel to user space
	  	NewPos = fileSystem->openf[id]->GetCurrentPos();
		machine->System2User(buffer, NewPos - OldPos +1, buf);
		machine->WriteRegister(2, NewPos - OldPos + 1);
	}
	else
	{
		machine->WriteRegister(2, -1);
		delete[] buf;
		return -1;
	}
	// read data from console 
	
	/*  
	if (fileOpen.type == 2)
	{
		int sz = gSynchConsole->Read(buf, charCount);
		machine->System2User(buffer, sz, buf);
		machine->WriteRegister(2, sz);
	}*/
	delete[] buf;
	return 1;
}
