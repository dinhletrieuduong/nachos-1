// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
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
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void 
SyscallHandler()
{
    int type = machine->ReadRegister(2);

    switch(type)
    {
        case SC_Halt:
            DEBUG('a', "Shutdown, initiated by user program.\n");
   	        interrupt->Halt();
            break;

        case SC_Create:
            machine->WriteRegister(2, SCF_Create());
            break;

        case SC_Open:
            // @FIXME: open for stdin & stdout
            machine->WriteRegister(2, SCF_Open());
            break;

        case SC_Write:
            // @FIXME: write to console
            SCF_Write();
            break;

        case SC_Read:
            // @FIXME: read from console
            machine->WriteRegister(2, SCF_Read());
            break;

        case SC_Seek:
            machine->WriteRegister(2, SCF_Seek());
            break;

        case SC_Close:
            SCF_Close();
            break;
            
        case SC_Writeln:
            SCF_Writeln();
            break;
            
        case SC_Exec:
            machine->WriteRegister(2, SCF_Exec());
            break;

        case SC_Join:
            machine->WriteRegister(2, SCF_Join());
            break;

        case SC_Exit:
            machine->WriteRegister(2, SCF_Exit());
            break;

        case SC_CreateSemaphore:
            machine->WriteRegister(2, SCF_CreateSemaphore());
            break;

        case SC_Up:
            machine->WriteRegister(2, SCF_Up());
            break;
            
        case SC_Down:
            machine->WriteRegister(2, SCF_Down());
            break;

        default:
            printf("Syscall %d not found\n", type);
            ASSERT(FALSE);
            break;
    }
    // printf("Syscall was called %d\n", type);
    // printf("Increase PC Register\n");
    machine->registers[PrevPCReg] = machine->registers[PCReg];
    machine->registers[PCReg] = machine->registers[NextPCReg];
    machine->registers[NextPCReg] += 4;
}

void
ExceptionHandler(ExceptionType which)
{
    switch (which)
    {
    case SyscallException:
        SyscallHandler();
        break;

    case PageFaultException:
        break;

    case ReadOnlyException:
        break;

    case BusErrorException:
        break;

    case AddressErrorException:
        break;

    case OverflowException:
        break;

    case IllegalInstrException:
        break;

    case NumExceptionTypes:
        break;



    default:
        printf("Unexpected user mode exception %d\n", which);
        ASSERT(FALSE);
        break;

    }
}
