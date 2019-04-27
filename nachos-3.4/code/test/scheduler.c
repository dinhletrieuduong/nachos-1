#include "syscall.h"

int main()
{
    OpenFileId stdout = ConsoleOutput;
    Writeln("Wait!");
    Exec("./test/ping");
    // @FIXME :((
    Exec("./test/pong");
    while(1);
    return 0;
}