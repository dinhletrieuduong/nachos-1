#include"stable.h"

STable::STable()
{
	bm = new BitMap(MAX_SEMAPHORE);

	for (int i = 0; i < 10; i++) 
	{
		semTab[i] = NULL;
	}
}
STable::~STable()
{
	if (bm != NULL)
	{
		delete bm;
	}

	for (int i = 0; i < MAX_SEMAPHORE; i++)
	{
		if (bm->Test(i))
		{
			delete semTab[i];
		}
	}
}

// Kiem tra Semaphore "name"chua ton tai thi tao Semaphore moi
int STable::Create(char* name, int init)
{
	for (int i = 0; i < MAX_SEMAPHORE && IsExist(i); i++) 
	{
		if (strcmp(name, semTab[i]->GetName()) != 0)
			return -1;
	}

	int position = FindFreeSlot();
	semTab[position] = new Sem(name, init);
}

int STable::Wait(char* name)
{
	for (int i = 0; i < MAX_SEMAPHORE && IsExist(i); i++) 
	{
		if (strcmp(name, semTab[i]->GetName()) == 0)
		{
			semTab[i]->Wait();
			return 0;
		}
	}

	return -1;
}

int STable::Signal(char* name)
{
	for (int i = 0; i < MAX_SEMAPHORE && IsExist(i); i++) 
	{
		if (strcmp(name, semTab[i]->GetName()) == 0)
		{
			semTab[i]->Signal();
			return 0;
		}
	}

	return -1;
}

int STable::FindFreeSlot()
{
	int slot = bm->Find();
	bm->Mark(slot);
	return slot;
}

bool STable::IsExist(int id)
{
	return bm->Test(id);
}