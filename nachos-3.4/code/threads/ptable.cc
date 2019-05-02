#include "ptable.h"
#include "system.h"

PTable::PTable(int size) {
	bm = new BitMap(size);
	bmsem = new Semaphore("bmsem", 1);

	for (int i = 0; i < MAX_PROCESS; i++)
		pcb[i] = NULL;
	bm->Mark(0);

	pcb[0] = new PCB(0);
	pcb[0]->parentID = -1;
}

PTable::~PTable() {
	if (bm != NULL) delete bm;
    if (bmsem != NULL) delete bmsem;
    for (int i = 0; i < MAX_PROCESS; ++i) {
		// Dung` IsExist
		// Kiem tra = NULL la sai
		// Mai delete no' chu' chua he` gan' no' = NULL
        if (IsExist(i)) 
        	delete pcb[i];
    }
}

int PTable::ExecUpdate(char *filename) {
	bmsem->P(); //chi nap 1 tien trinh vao mot thoi diem

	if (filename == NULL) {
		printf("\nFile name is null\n");
        bmsem->V();
        return -1;
    }

	//Kiem tra file co ton tai tren may khong
	OpenFile *execution = fileSystem->Open(filename);
	if (execution == NULL) {
		printf("\nUnable to open file %s\n", filename);
		bmsem->V();
		return -1;
	}
	delete execution; // close

	//Kiem tra chuong trinh duoc goi co la chinh no khong
	if (strcmp(filename, currentThread->getName()) == 0) {
		printf("\nError: Process tries to execute itself\n");
		bmsem->V();
		return -1;
	}

	//Kiem tra con slot trong khong
	int id = GetFreeSlot();
	if (id < 0) {
		printf("\nError: No more space for new process\n");
		bmsem->V();
		return -1;
	}

	pcb[id] = new PCB(id);
	bm->Mark(id);
	pcb[id]->SetFileName(filename);
    pcb[id]->parentID = currentThread->processID;
	int pID = pcb[id]->Exec(filename, id);

	bmsem->V();
	return pID;
}

int PTable::JoinUpdate(int pID) {
	// Ta kiểm tra tính hợp lệ của processID id và kiểm tra tiến trình gọi Join có phải là cha của tiến trình
	// có processID là id hay không. Nếu không thỏa, ta báo lỗi hợp lý và trả về -1.
	if (pID < 0 || pID > 9) {
		printf("\nError: Unavailable process: id = %d\n", pID);
		return -1;
	}
	if (pcb[pID] == NULL) {
		printf("\nError: Unavailable process\n");
		return -1;
	}

	//kiem tra tien trinh dang chay co la cha cua tien trinh can join hay khong
	if (currentThread->processID != pcb[pID]->parentID) {
		printf("\nError: Process tries to join not parent process\n");
		return -1;
	}
	// Tăng numwait và gọi JoinWait() để chờ tiến trình con thực hiện. Sau khi tiến trình con thực hiện xong, tiến trình đã được giải phóng
	pcb[pcb[pID]->parentID]->IncNumWait();
	pcb[pID]->JoinWait(); //doi den khi tien trinh con ket thuc
	int ec = pcb[pID]->GetExitCode();
	pcb[pID]->ExitRelease(); //cho phep tien trinh con ket thuc
	return ec;
}

int PTable::ExitUpdate(int ec) {
	int pID = currentThread->processID;

	printf("\nPTable::ExitUpdate, PID: %d  |  Exit: %d\n", pID, ec);
	//Neu la main process thi Halt()
	if (pID == 0) {
    	currentThread->FreeSpace();	
		interrupt->Halt();
		return 0;
	}
	if (IsExist(pID) == false) {
		printf("\nError: Unavailable process\n");
		return -1;
	}
	printf("P: %d  |  ParentID: %d\n", pcb[pID], pcb[pID]->parentID);
	pcb[pID]->SetExitCode(ec);
	pcb[pcb[pID]->parentID]->DecNumWait();
	pcb[pID]->JoinRelease();
	pcb[pID]->ExitWait();
	Remove(pID);
	return ec;
}

void PTable::Remove(int pID) {
    bm->Clear(pID);
	if(pcb[pID] != 0)
		delete pcb[pID];
}

int PTable::GetFreeSlot() {
	return bm->Find();
}

bool PTable::IsExist(int pID) {
	return bm->Test(pID);
}

char *PTable::GetFileName(int pID) {
	return pcb[pID]->GetFileName();
}
