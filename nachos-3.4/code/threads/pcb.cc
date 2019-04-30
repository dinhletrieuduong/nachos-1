#include "pcb.h"
#include "utility.h"
#include "system.h"
#include "thread.h"
#include "addrspace.h"

PCB::PCB(int id) {
	joinsem = new Semaphore("JoinSem",0);
	exitsem = new Semaphore("ExitSem",0);
	mutex = new Semaphore("Mutex",1);
	
	exitcode = 0;
    numwait = 0;
	pid = id;
	if(id == 0)
		parentID = -1;
	else
		parentID = currentThread->processID;
	thread = NULL;
}

PCB::~PCB() {
	printf("PCB::~PCB, free space\n");
	// thread->FreeSpace();
    delete joinsem;
    delete exitsem;
    delete mutex;
}

int PCB::Exec(char *filename, int pID) {
	mutex->P();
	
	thread = new Thread(filename);
	if(thread == NULL){
		printf("\nError: Not enough memory..!\n");
        mutex->V();
		return -1;
	}
	if (filename == NULL || pID < 0) {
		printf("\nError: File name or id is unvalid...\n");
        mutex->V();
        return -1;
    }
    /*OpenFile *execution = fileSystem->Open(filename);
	AddrSpace* space = new AddrSpace(execution);
	
	if(space == NULL) {
		printf("\nError: Not enough space...\n");
		mutex->V();
		return -1; 
	}
    delete space;
	delete execution;*/

	thread->processID = pID;
	pid = pID;
	parentID = currentThread->processID;
	// cast thread thành kiểu int, sau đó khi xử ký hàm StartProcess ta cast Thread về đúng kiểu của nó
	thread->Fork(StartProcess_2, pID);
	mutex->V();
	return pID;
}

int PCB::GetID() {
	return pid;
}

int PCB::GetNumWait() {
	return numwait;
}

void PCB::JoinWait() {
	joinsem->P();
}

void PCB::ExitWait() {
	exitsem->P();
}

void PCB::JoinRelease() {
	joinsem->V();
}

void PCB::ExitRelease() {
	exitsem->V();
}

void PCB::IncNumWait() {
	mutex->P();
	++numwait;
	mutex->V();
}

void PCB::DecNumWait() {
	mutex->P();
	if(numwait > 0)
		--numwait;
	mutex->V();
}

int PCB::GetExitCode() {
	return exitcode;	
}

void PCB::SetExitCode(int ec) {
	exitcode = ec;
}
void PCB::SetFileName(char* fn){ 
	strcpy(fileName,fn);
}
char* PCB::GetFileName() {
	return fileName;
}

/*************************************************************************************/
void StartProcess_2(int id) {
    char* fileName = gPTable->GetFileName(id);
    OpenFile *executable = fileSystem->Open(fileName);

    if (executable == NULL) {
	    printf("\nUnable to open file %s\n", fileName);
	    return;
    }
    AddrSpace *space;
    space = new AddrSpace(executable);
    //space = new AddrSpace(fileName);

	if(space == NULL)
	{
		printf("\nCan't create AddSpace.\n");
		return;
	}

    currentThread->space = space;

    space->InitRegisters();		
    space->RestoreState();		

    currentThread->space->InitRegisters();		// set the initial register values
	currentThread->space->RestoreState();		// load page table register
    
	machine->Run();			// jump to the user progam
delete space;
	ASSERT(FALSE);			// machine->Run never returns;
							// the address space exits
							// by doing the syscall "exit"

}
