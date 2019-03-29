// exception.cc 
//  Entry point into the Nachos kernel from user programs.
//  There are two kinds of things that can cause control to
//  transfer back to here from user code:
//
//  syscall -- The user code explicitly requests to call a procedure
//  in the Nachos kernel.  Right now, the only function we support is
//  "Halt".
//
//  exceptions -- The user code does something that the CPU can't handle.
//  For instance, accessing memory that doesn't exist, arithmetic errors,
//  etc.  
//
//  Interrupts (which can also cause control to transfer from user
//  code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "syscallfuncs.h"

//----------------------------------------------------------------------
// ExceptionHandler
//  Entry point into the Nachos kernel.  Called when a user program
//  is executing, and either does a syscall, or generates an addressing
//  or arithmetic exception.
//
//  For system calls, the following is the calling convention:
//
//  system call code -- r2
//      arg1 -- r4
//      arg2 -- r5
//      arg3 -- r6
//      arg4 -- r7
//
//  The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//  "which" is the kind of exception.  The list of possible exceptions 
//  are in machine.h.
//----------------------------------------------------------------------

void SyscallHandler()
{
    int type = machine->ReadRegister(2);
    switch(type)
    {
        case SC_Halt:
            DEBUG('a', "Shutdown, initiated by user program.\n");
            interrupt->Halt();
            break;
            
        case SC_Exit:
            break;
            
        case SC_PrintInt:
            SCF_PrintInt();
            break;
        
        case SC_PrintChar:
            SCF_PrintChar();
            break;
            
        case SC_PrintString:
            SCF_PrintString();
            break;
            
        case SC_ReadString:
            machine->WriteRegister(2, SCF_ReadString());
            break;

        case SC_Exec:
            machine->WriteRegister(2, SCF_ExecCmd());
            break;

        case SC_CreateFile:
            machine->WriteRegister(2, SCF_CreateFile());
            break;
            
        case SC_Create:
            machine->WriteRegister(2, SCF_CreateFile());
            break;
            
        case SC_OpenFileID:
            machine->WriteRegister(2, SCF_OpenFileID());
            break;
            
        case SC_Close:
            SCF_CloseFile();
            break;
            
        case SC_ReadFile:
            machine->WriteRegister(2, SCF_ReadFile());
            break;
            
        case SC_Read:
            machine->WriteRegister(2, SCF_ReadFile());
            break;

        case SC_WriteFile:
            machine->WriteRegister(2, SCF_WriteFile());
            break;
            
        case SC_Write:
            machine->WriteRegister(2, SCF_WriteFile());
            break;

        case SC_SeekFile:
            machine->WriteRegister(2, SCF_SeekFile());
            break;
            
        default:
            printf("Unexpected syscall %d\n", type);
            break;
    }
    
    machine->registers[PrevPCReg] = machine->registers[PCReg];
    machine->registers[PCReg] = machine->registers[NextPCReg];
    machine->registers[NextPCReg] += 4;
}

void ExceptionHandler(ExceptionType which)
{
    switch(which)
    {
        case SyscallException:
            SyscallHandler();
            break;
        
        case NoException:
            return;
        case PageFaultException:
            printf("No valid translation found.\n");
            interrupt->Halt();
            break;
        
        case ReadOnlyException:
            printf("Write attempted to page marked \'read-only\'.");
            interrupt->Halt();
            break;
            
        case BusErrorException:
            printf("Translation resulted in an invalid physical address.\n");
            interrupt->Halt();
            break;
            
        case AddressErrorException:
            printf("Unaligned reference or one that was beyond the end of the address space.\n");
            interrupt->Halt();
            break;
            
        case OverflowException:
            printf("Integer overflow!!!.\n");
            interrupt->Halt();
            break;
            
        case IllegalInstrException:
            printf("Unimplemented or reserved instruction.\n");
            interrupt->Halt();
            break;   
        
        case NumExceptionTypes:
            printf("Cai nay la cai gi day.\n");
            interrupt->Halt();
            break;
    }
}
