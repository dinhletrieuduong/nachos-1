#include "syscall.h"

int
main()
{
    SpaceId newProc;
    OpenFileId input = ConsoleInput;
    OpenFileId output = ConsoleOutput;
    char prompt[2], ch, buffer[60];
    int i;

    prompt[0] = '-';
    prompt[1] = '-';

    while( 1 )
    {
	Write(prompt, 2, output);

	PrintString("User Input >>> ");
	i = ReadString(buffer);

	if( i > 0 ) {
		newProc = Exec(buffer);
		Join(newProc);
	}
    }
}

