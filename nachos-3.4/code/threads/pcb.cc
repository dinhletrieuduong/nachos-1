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
	if(id)
		parentID = currentThread->pid;
	else
		parentID = -1;
	thread = NULL;
}

PCB::~PCB() {
    delete joinsem;
    delete exitsem;
    delete mutex; 
	//thread->FreeSpace();
	//thread->Finish();
}

int PCB::Exec(char *filename, int pID) {
	mutex->P();
	
	thread = new Thread(filename);
	if(this->thread == NULL){
		printf("\nPCB::Exec:: Not enough memory..!\n");
        	mutex->V();
		return -1;
	}
	if (filename == NULL || pID < 0) {
		printf("\nError: File name is null or id is unvalid...\n");
        mutex->V();
        return -1;
    }

	thread->pid = pID;
	pid = pID;
	parentID = currentThread->pid;
	
	OpenFile *execution = fileSystem->Open(filename);
	AddrSpace* space = new AddrSpace(execution);
	
	if(space == NULL) {
		printf("\nError: Not enough space...\n");
		mutex->V();
		return -1; 
	}
	delete execution;
	delete space;
	
	// cast thread thành kiểu int, sau đó khi xử ký hàm StartProcess ta cast Thread về đúng kiểu của nó
	thread->Fork(StartProcess_2, pID);
	mutex->V();
	return pID;
}

int PCB::GetID() {
	//return pid;
	return thread->pid;
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
void StartProcess_2(int) {
	currentThread->space->InitRegisters();		// set the initial register values
	currentThread->space->RestoreState();		// load page table register

	machine->Run();			// jump to the user progam
	ASSERT(FALSE);			// machine->Run never returns;
							// the address space exits
							// by doing the syscall "exit"
}
