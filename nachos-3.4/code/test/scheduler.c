#include "syscall.h"

int main()
{
    OpenFileId stdout = ConsoleOutput;
    int ping;
    int pong;
    Writeln("Pingpong...");
    ping = Exec("./test/ping");
    pong = Exec("./test/pong");
    Join(ping);
    Join(pong);
    return 0;
}