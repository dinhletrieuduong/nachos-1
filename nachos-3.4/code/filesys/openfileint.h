#ifndef OPENFILEINT_H
#define OPENFILEINT_H

class OpenFileInt {
public:
    virtual ~OpenFileInt() { };
    virtual int Read(char *into, int numBytes) = 0;
    virtual int Write(char *from, int numBytes) = 0;
    virtual int GetCurrentPos() { return 0; }
    virtual int Length() { return 0; }
};

#endif