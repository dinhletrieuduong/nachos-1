#include "ftable.h"

FTable::FTable(int numMaxFile)
{
    mNumMaxFile = numMaxFile;
    mFileTable = new OpenFileInt*[numMaxFile];
    mBitmap = new BitMap(numMaxFile);
    mFileSystem = new FileSystem(TRUE);
    
    for (int i = 0; i < mNumMaxFile; ++i)
    {
        mFileTable[i] = NULL;
    }

    mFileTable[ConsoleInput] = new SynchConsole();
    mFileTable[ConsoleOutput] = mFileTable[ConsoleInput];
    mBitmap->Mark(0);
    mBitmap->Mark(1);
}

FTable::~FTable()
{
    delete mFileTable[ConsoleInput];

    for (int i = 2; i < mNumMaxFile; ++i)
    {
        Close(i);
    }

    delete[] mFileTable;
}

OpenFileId FTable::Open(char* filename, int type)
{
    int index = mBitmap->Find();

    if (index < 2)
    {
        return -1;
    }

    if (mFileTable[index] != NULL)
    {
        // printf("FTable::Open, file is already opened!\n");
        return -1;
    }

    if (type != 0 && type != 1)
    {
        // printf("FTable::Open, unexpected type (0:rw, 1:r)\n");
        return -1;
    }

    mFileTable[index] = mFileSystem->Open(filename, type);

    if (mFileTable[index] == NULL)
    {
        Close(index);
        // printf("FTable::Open, failed to open file!\n");
        return -1;
    }

    // printf("FTable::Open, open file index %d\n", index);
    mBitmap->Mark(index);
    return index;    
}

void FTable::Close(OpenFileId index)
{
    if (index < 2 || index >= mNumMaxFile)
    {
        // printf("FTable::Close, index out of range!\n");
        return;
    }

    if (mFileTable[index] == NULL)
    {
        // printf("FTable::Close, file is not opened!\n");
        return;
    }

    // printf("FTable::Close, close file index %d\n", index);

    mBitmap->Clear(index);
    delete mFileTable[index];
    mFileTable[index] = NULL;
}

int FTable::Read(char* buffer, int numBytes, OpenFileId fileid)
{
    if (fileid < 0 || fileid >= mNumMaxFile)
    {
        // printf("FTable::Read, index out of range!\n");
        return -1;
    }

    if (mFileTable[fileid] == NULL)
    {
        // printf("FTable::Read, file is not opened!\n");
        return -1;
    }

    if (fileid != ConsoleInput && mFileTable[fileid]->GetCurrentPos() == Seek(fileid, -1))
    {
        // printf("FTable::Read, could not read, EOF\n");
        return -2;
    }

    return mFileTable[fileid]->Read(buffer, numBytes);
}

int FTable::Write(char* buffer, int numBytes, OpenFileId fileid)
{
    if (fileid < 0 || fileid >= mNumMaxFile)
    {
        // printf("FTable::Write, index out of range!\n");
        return -1;
    }

    if (mFileTable[fileid] == NULL)
    {
        // printf("FTable::Write, file is not opened!\n");
        return -1;
    }

    if (fileid == ConsoleInput)
    {
        return -1;
    }

    return mFileTable[fileid]->Write(buffer, numBytes);
}

int FTable::Seek(int pos, OpenFileId fileid)
{
    int len;

    if (fileid < 2 || fileid >= mNumMaxFile)
    {
        // printf("FTable::Seek, index out of range");
        return -1;
    }

    if (mFileTable[fileid] == NULL)
    {
        return -1;
    }

    len = mFileTable[fileid]->Length();
    
    if (pos == -1)
    {
        return len;
    }
    else if (pos >= len || pos < 0)
    {
        return -1;
    }
    else
    {
        ((OpenFile*)mFileTable[fileid])->Seek(pos);
    }


    return pos;
}