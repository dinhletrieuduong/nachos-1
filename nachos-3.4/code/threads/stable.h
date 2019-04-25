#ifndef STABLE_H
#define STABLE_H
#define MAX_SEMAPHORE 10

#include "thread.h"
#include "synch.h"
#include "sem.h"
#include "bitmap.h"

class Stable {
private:
	BitMap* bm;							// quản lý slot trống
	Sem* semTab[MAX_SEMAPHORE];			// quản lý tối đa 10 đối tượng Sem
public:
	// khởi tạo size đối tượng Sem để quản lý 10 Semaphore. Gán giá trị ban đầu là null
	// nhớ khởi tạo bm để sử dụng
	STable();							// hủy các đối tượng đã tạo
	~STable();
	int Create(char* name, int init);	// Kiểm tra Semaphore “name” chưa tồn tại thì tạo Semaphore mới. Ngược lại, báo lỗi.
	int Wait(char* name);				// Nếu tồn tại Semaphore “name” thì gọi this->P()để thực thi. Ngược lại, báo lỗi
	int Signal(char* name);				// Nếu tồn tại Semaphore “name” thì gọi this->V()để thực thi. Ngược lại, báo lỗi.
	int FindFreeSlot(int id);			// Tìm slot trống.
};

#endif