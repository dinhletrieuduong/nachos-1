#ifndef FTABLE_H
#define FTABLE_H

#include "syscall.h"
#include "openfile.h"
#include "bitmap.h"
#include "filesys.h"
#include "synchcons.h"

class FTable
{
public:
    FTable(int numMaxFile);

    ~FTable();

    OpenFileId Open(char* filename, int type);

    void Close(OpenFileId);

    int Read(char* buffer, int numBytes, OpenFileId fileId);

    int Write(char* buffer, int numBytes, OpenFileId fileid);

    int Seek(int pos, OpenFileId fileid);
private:
    int mNumMaxFile;
    BitMap* mBitmap;
    OpenFileInt** mFileTable;
    FileSystem* mFileSystem;
};

#endif