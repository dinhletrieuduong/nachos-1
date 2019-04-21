#include "syscall.h"

int main()
{
    OpenFileId stdout = ConsoleOutput;
    Exec("./test/ping");
    // @FIXME :((
    Exec("./test/pong");
    while(1);
    return 0;
}