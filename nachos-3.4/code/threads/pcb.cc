#include "pcb.h"
#include "utility.h"
#include "system.h"
#include "thread.h"
#include "addrspace.h"

PCB::PCB() {
	joinsem = new Semaphore("JoinSem",0);
	exitsem = new Semaphore("ExitSem",0);
	mutex = new Semaphore("Mutex",1);
	
	exitcode = 0;
	numwait = 0;
	pid = -1;
	parentID = -1;
	thread = NULL;
}

PCB::PCB(int id) {
	joinsem = new Semaphore("JoinSem",0);
	exitsem = new Semaphore("ExitSem",0);
	mutex = new Semaphore("Mutex",1);
	
	exitcode = 0;
	numwait = 0;
	pid = id;
	if(id)
		parentID = currentThread->ID;
	else
		parentID = 0;
	thread = NULL;
}

PCB::~PCB()
{
	if(joinsem != NULL)
		delete joinsem;
	if(exitsem != NULL)
		delete exitsem;
	if(mutex != NULL)
		delete mutex;
}

char* PCB::GetFileName() {
	return thread->getName();
}

int PCB::Exec(char *filename, int pID) {
	mutex->P();
	if (filename == NULL || pID < 0 || thread != NULL) {
		printf("\nError: File name is null or id is negative or can't create new thread...\n");
        mutex->V();
        return -1;
    }
	thread = new Thread(filename);
	thread->ID = pID;
	pid = pID;
	OpenFile *execution = fileSystem->Open(filename);
	AddrSpace *space = new AddrSpace(execution);
	
	if(space == NULL) {
		printf("\nError: Not enough space...\n");
		mutex->V();
		return -1; 
	}
	currentThread->space = space;
	delete executable;
	delete space;
	thread->Fork(StartProcess,pID);
	mutex->V();
	return pID;
}

int PCB::GetID() {
	return pid;
	//return thread->ID;
}

int PCB::GetNumWait() {
	return numwait;
}


void PCB::JoinWait() {
	IncNumWait();
	joinsem->P();
}

void PCB::ExitWait() {
	exitsem->P();
}

void PCB::JoinRelease() {
	DecNumWait();
	joinsem->V();
}

void PCB::ExitRelease() {
	exitsem->V();
}

void PCB::IncNumWait() {
	++numwait;
}

void PCB::DecNumWait() {
	if(numwait)
		--numwait;
}

int PCB::GetExitCode() {
	return exitcode;	
}

void PCB::SetExitCode(int ec) {
	exitcode = ec;
}

/*************************************************************************************/
void StartProcess(int) {
	currentThread->space->InitRegisters();		// set the initial register values
	currentThread->space->RestoreState();		// load page table register

	machine->Run();			// jump to the user progam
	ASSERT(FALSE);			// machine->Run never returns;
							// the address space exits
							// by doing the syscall "exit"
}
