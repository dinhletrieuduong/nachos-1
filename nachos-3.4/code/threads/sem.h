#ifndef SEM_H
#define SEM_H

#include "synch.h"

#define SEM_MAXNAMESIZE 50	// the max size of the name of semaphore

class Sem {
private:
    char name[50];
    Semaphore *sem;			// Tạo Semaphore để quản lý
public:
	// khởi tạo đối tượng Sem. Gán giá trị ban đầu là null
	// nhớ khởi tạo bm sử dụng
    Sem(char* na, int i);
    ~Sem();					// hủy các đối tượng đã tạo
    int Create(char *name, int semval);    // create a new semaphore. return 0 if success and -1 if failure
    void Delete();                         // delete the semaphore if exist
    void Wait();			// thực hiện thao tác chờ
    void Signal();			// thực hiện thao tác giải phóng Semaphore
    char* GetName();		// Trả về tên của Semaphore
};

#endif
