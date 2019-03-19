#include "syscallfuncs.h"

void SCF_ExecCmd()
{
    static const int MaxCmdLength = 64;
    int virtAddr = machine->ReadRegister(4);
    char* cmd = machine->User2System(virtAddr, MaxCmdLength + 1);
    
    if (cmd == NULL)
    {
        printf("Not enough memory in system\n");
        machine->WriteRegister(2, -1);
        delete cmd;
        return;
    }
    
    printf("Executing \'%s\'\n", cmd);
    machine->WriteRegister(2, 0);
}

int SCF_CreateFile()
{
    static const int MaxFileLength = 32;
    int virtAddr = machine->ReadRegister(4);
    char* fileName = machine->User2System(virtAddr, MaxFileLength + 1);
    
    if (fileName == NULL)
    {
        printf("Not enough memory in system\n");        
        delete fileName;
        return -1;
    }
    
    printf("Create new file \'%s\'\n", fileName);
    
    if (!fileSystem->Create(fileName, 0))
    {
        printf("Error while creating file \'%s\'\n", fileName);
        delete fileName;
        return -1;
    }
    
    delete fileName;
	return 0;
}

int SCF_OpenFileID(){
	int bufAddr = machine->ReadRegister(4); // read string pointer from user
	int type = machine->ReadRegister(5);
	char *buf = new char[LIMIT];
	if (fileSystem->index > 10)
	{
		machine->WriteRegister(2, -1);
		delete[] buf;
		return -1;
	}
	buf = machine->User2System(bufAddr, LIMIT);
	if (strcmp(buf,"stdin") == 0)
	{
		printf("stdin mode\n");
		machine->WriteRegister(2, 0);
		return 0;
	}
	if (strcmp(buf,"stdout") == 0)
	{
		printf("stdout mode\n");
		machine->WriteRegister(2, 1);
		return 1;
	}
	if ((fileSystem->openf[fileSystem->index] = fileSystem->Open(buf, type)) != NULL)
	{
		DEBUG('f',"open file successfully");
		machine->WriteRegister(2, fileSystem->index-1);
		delete [] buf;
		return fileSystem->index-1;
	} else 
	{
		DEBUG('f',"can not open file");
		machine->WriteRegister(2, -1);
		delete [] buf;
		return -1;
	};
}
void SCF_ReadFile(){
	int buffer = machine->ReadRegister(4);
	int charCount = machine->ReadRegister(5);
	int id = machine->ReadRegister(6);	
	int OldPos;
	int NewPos;
	char *buf = new char[charCount];
	int i = 0;
	// Check id
	// Do toi da la 10 file, > 10 thi dung`
	if (id < 0 || id > 10)
	{
		delete[] buf;
		return -1;
	}
	// check openf[id]
	if (fileSystem->openf[id] == NULL) // kiem tra xem file co mo thanh cong k, thanh cong != NULL
	{
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
		return sz;
	}
	
	if ((fileSystem->openf[id]->Read(buf, charCount) ) > 0)
	{
		// Copy data from kernel to user space
	  	NewPos = fileSystem->openf[id]->GetCurrentPos();
		machine->System2User(buffer, NewPos - OldPos +1, buf);
		return NewPos - OldPos + 1;
	}
	else
	{
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

int SCF_WriteFile() {
	int buffer = machine->ReadRegister(4);
	int charCount = machine->ReadRegister(5);
	int id =  machine->ReadRegister(6);
	int OldPos;
	int NewPos;
	char *buf = new char[charCount];
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
	
	// type must equals '0'
	buf = machine->User2System(buffer, charCount);
	if (fileSystem->openf[id]->type  == 0 || fileSystem->openf[id]->type == 3)
	{	
	if ((fileSystem->openf[id]->Write(buf, charCount)) > 0) 
	{
		// Copy data from kernel to user space
		printf("%s",buf);
		NewPos = fileSystem->openf[id]->GetCurrentPos();
		machine->WriteRegister(2, NewPos - OldPos + 1);
	}
	else if (fileSystem->openf[id]->type == 1);
	{
		machine->WriteRegister(2, -1);
		delete[] buf;
		return -1;
	}
	}
	// Write data to console
	if (fileSystem->openf[id]->type == 3)
	{
		int i = 0;
		printf("stdout mode\n");
		while (buf[i] != 0 && buf[i] != '\n')
		{
			gSynchConsole->Write(buf+i, 1);
			i++;
		}
		buf[i] = '\n';
		gSynchConsole->Write(buf+i,1);
		machine->WriteRegister(2, i-1);
	}
	delete[] buf;
	return 1;
}

int SCF_SeekFile(){
	int bufAddr = machine->ReadRegister(4);
	int NumBuf = machine->ReadRegister(5);
	int m_index =  machine->ReadRegister(6);
	int OldPos;
	int NewPos;
	char *buf = new char[NumBuf];
	// Check m_index
	if (m_index < 0 || m_index > 10)
	{
		machine->WriteRegister(2, -1);
		delete[] buf;
		return -1;
	}
	// check openf[m_index]
	if (fileSystem->openf[m_index] == NULL)
	{
		machine->WriteRegister(2, -1);
		delete[] buf;
		return -1;
	}
	OldPos = fileSystem->openf[m_index]->GetCurrentPos();
	
	// type must equals '0'
	buf = machine->User2System(bufAddr, NumBuf);
	if (fileSystem->openf[m_index]->type  == 0 || fileSystem->openf[m_index]->type == 3)
	{	
	if ((fileSystem->openf[m_index]->Write(buf, NumBuf)) > 0) 
	{
		// Copy data from kernel to user space
		printf("%s",buf);
		NewPos = fileSystem->openf[m_index]->GetCurrentPos();
		machine->WriteRegister(2, NewPos - OldPos + 1);
	}
	else if (fileSystem->openf[m_index]->type == 1);
	{
		machine->WriteRegister(2, -1);
		delete[] buf;
		return -1;
	}
	}
	// Write data to console
	int i = 0;
	if (fileSystem->openf[m_index]->type == 3)
	{
		printf("stdout mode\n");
		while (buf[i] != 0 && buf[i] != '\n')
		{
			gSynchConsole->Write(buf+i, 1);
			i++;
		}
		buf[i] = '\n';
		gSynchConsole->Write(buf+i,1);
		machine->WriteRegister(2, i-1);
	}
	delete[] buf;
	return i-1;
}

void SCF_PrintChar()
{
    char ch;
    ch = (char) machine->ReadRegister(4);
    gSynchConsole->Write(&ch, 1);
}

void SCF_PrintString()
{
    int bufAddr = machine->ReadRegister(4);
    int i = 0;
    char *buf = new char[LIMIT];
    buf = machine->User2System(bufAddr, LIMIT);
    while (buf[i] != 0 && buf[i] != '\n')
    {
        gSynchConsole->Write(buf+i, 1);
        i++;
    }
    buf[i] = '\n';
    gSynchConsole->Write(buf+i,1);
    delete[] buf;
}



