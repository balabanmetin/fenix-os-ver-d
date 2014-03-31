/*! \file sysdefines.h
 *  This file defines constants and data types used by system calls.
 *
 */

#ifndef _SYSDEFINES_H_
#define _SYSDEFINES_H_

/* Constant declarations. */

/*! Return code when system call returns normally. */
#define ALL_OK                  (0)
/*! Return code when system call returns with an error. */
#define ERROR                   (-1)
/*! Return code when system call is unknown. */
#define ERROR_ILLEGAL_SYSCALL   (-2)
/*! Max number of columns in the VGA buffer. */
#define MAX_COLS                (80)
/*! Max number of columns in the VGA buffer. */
#define MAX_ROWS                (25)

/*! System call that returns the version of the kernel. */
#define SYSCALL_VERSION         (0)
/*! System call that prints a string. */
#define SYSCALL_PRINTS          (1)
/*! System call that prints a hexadecimal value. */
#define SYSCALL_PRINTHEX        (2)
/*! System call that pauses execution by invoking the bochs debugger. */
#define SYSCALL_DEBUGGER        (3)

/*! System call that allocates a memory block. The length of the requested
    memory block is passed in rdi. The system call returns the address or an
    error code. */
#define SYSCALL_ALLOCATE        (4)

/*! System call that frees a memory block allocated through the allocate
    system call. The address of the memory block is passed in rdi. The
    system call returns  ALL_OK if successful or an error code if
    unsuccessful. */
#define SYSCALL_FREE            (5)

/*! System call that terminates the currently running
 *  thread. It takes no parameters. Terminates the process
 *  when there are no threads left. */
#define SYSCALL_TERMINATE       (6)

/*! System call that creates a new process with one single
 *  thread. It takes an index into the executable table in
 *  rdi. The program used is the executable whose index is
 *  passed in rdi. */
#define SYSCALL_CREATEPROCESS   (7)

/*! System call that blocks the calling thread a number of clocks ticks. The
   number of clock ticks is passed in rdi. There are 200 clock
   ticks per second.*/
#define SYSCALL_PAUSE           (8)

/*! System call that returns the current system time. The system time is the
    number of clock ticks since system start. There are 200 clock ticks per
    second. */
#define SYSCALL_TIME            (9)

/*! Creates a new thread in the process of the calling thread. The value of
    rip register of the new thread is passed in the rdi registe. The value of
    the rsp register of the new thread is passed in rsi. rflags of the new
    thread is set to 0x200. The other registers are undefinied.

    The system call returns in rax ALL_OK if successful or an error code if
    unsuccessful.
   */
#define SYSCALL_CREATETHREAD    (10)

/*! Creates a new semaphore and returns if successful the handle of the
    semaphore in the rax register. The initial value of the semaphore count
    value is passed in the rdi register.

    If unsuccessful the system call returns an error code in rax. */
#define SYSCALL_CREATESEMAPHORE (11)

/*! Performs a semaphore down operation. The handle of the semaphore is passed
    in the rdi register.

    The system call returns in rax ALL_OK if successful or an error code if
    unsuccessful.
   */
#define SYSCALL_SEMAPHOREDOWN   (12)

/*! Performs an semaphore up operation. The handle of the semaphore is passed
    in the rdi register.

    The system call returns in rax ALL_OK if successful or an error code if
    unsuccessful.
   */
#define SYSCALL_SEMAPHOREUP     (13)
#endif
