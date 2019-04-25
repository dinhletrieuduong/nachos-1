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
    void wait();			// thực hiện thao tác chờ
    void signal();			// thực hiện thao tác giải phóng Semaphore
    char* getName();		// Trả về tên của Semaphore
};

#endif