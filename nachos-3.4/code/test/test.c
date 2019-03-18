#include "syscall.h"
#include "copyright.h"
#define maxlen 32
int
main()
{
	/*char mess[255];
	int len;
	char filename[maxlen +1];
	//PrintfInt(123);
	
	OpenFileID fileId = OpenFileFunc("abc.txt", 1);
	ReadFile(mess, 255, fileId);
	PrintfChar(mess[0]);
	PrintfChar(mess[1]);
	PrintfString(mess);*/

	OpenFileId fileId;
	int filesz, i;
	char c;
	char name[255];
	//PrintfString("Input file name:");
	//ReadString(name, 255);
	if ( (fileId= OpenFileFunc("abc.txt", 1)) == -1)
	{
		PrintfString("Can not open file ");
		return 0;
	}
	filesz = SeekFile(-1, fileId);
	i = 0;
	SeekFile(0, fileId);
	
	for (; i < filesz; ++i)
	{
		ReadFile(&c, 1, fileId);
		PrintfChar(c);
	}
	return 0;
	Halt();
}
