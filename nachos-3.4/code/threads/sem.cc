#include "sem.h"

Sem::Sem(char* na, int i) {
	strcpy(this->name, na);
	sem = new Semaphore(name, i);
}
Sem::~Sem() {
	delete sem;
}
int Sem::Create(char *name, int semval) {
    if (name == NULL || semval < 0 || sem != NULL) return -1;
    strncpy(name, name, SEM_MAXNAMESIZE-1);
    sem = new Semaphore(name, semval);
    return 0;
}

void Sem::Delete() {
    if (sem == NULL) return;
    memset(name, 0, SEM_MAXNAMESIZE);
    delete sem;
    sem = NULL;
}
void Sem::wait() {
	sem->P();
}
void Sem::signal() {
	sem->V();
}
char* Sem::getName() {
	return name;
}
