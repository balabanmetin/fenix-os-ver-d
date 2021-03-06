/*! \file scwrapper.h
 *  This file contains C wrapper functions for system calls.
 *  The wrappers are used by user programs to perform system
 *  calls from C.
 */

#ifndef _SCWRAPPER_H_
#define _SCWRAPPER_H_

/* Include the constants that identifies system calls. */

#include "sysdefines.h"

/*! Wrapper for the system call that returns the version of the kernel. */
static inline unsigned long
version(void)
{
 unsigned long return_value;
 __asm volatile("syscall" :
                 "=a" (return_value) :
                 "a" (SYSCALL_VERSION) :
                 "cc", "%rcx", "%r11");
 return return_value;
}

/*! Wrapper for the system call that prints a string.
 * @param string Pointer to the string to be printed.
 * */
static inline long
prints(const char* const string)
{
 long return_value;
 __asm volatile("syscall" :
                 "=a" (return_value) :
                 "a" (SYSCALL_PRINTS), "D" (string) :
                 "cc", "%rcx", "%r11", "memory");
 return return_value;
}

/*! Wrapper for the system call that prints a hexadecimal value.
 * @param value hexadecimal value to be printed.
 */
static inline long
printhex(const unsigned long value)
{
 long return_value;
 __asm volatile("syscall" :
                 "=a" (return_value) :
                 "a" (SYSCALL_PRINTHEX), "D" (value) :
                 "cc", "%rcx", "%r11");
 return return_value;
}

/*! Wrapper for the system call that invokes the bochs debugger */
static inline long
debugger(void)
{
 long return_value;
 __asm volatile("syscall" :
                 "=a" (return_value) :
                 "a" (SYSCALL_DEBUGGER) :
                 "cc", "%rcx", "%r11");
 return return_value;
}

/*! Wrapper for the system call that allocates a memory block
 *  @param length integer holding the number of bytes to allocate
 */
static inline void *
alloc(unsigned long length)
{
 void * return_value;
 __asm volatile("syscall" :
                 "=a" (return_value) :
                 "a" (SYSCALL_ALLOCATE), "D" (length) :
                 "cc", "%r11", "%rcx");
 return return_value;
}

/*! Wrapper for the system call that frees a memory block.
 *  @param address address to the memory block to free.
 */
static inline long
free(void * address)
{
 long return_value;
 __asm volatile("syscall" :
                 "=a" (return_value) :
                 "a" (SYSCALL_FREE), "D" (address) :
                 "cc", "%r11", "%rcx");
 return return_value;
}

/*! Wrapper for the system call that terminates threads and processes. */
static inline void
terminate(void)
{
 __asm volatile("syscall" :
                 :
                 "a" (SYSCALL_TERMINATE) :
                 "cc", "%rcx", "%r11");
}

/*! Wrapper for the system call that creates processes.
 * @param executable integer identifying the program which should be loaded 
 *  and run as a process.
 */
static inline unsigned long
createprocess(const int executable)
{
 unsigned long return_value;
 __asm volatile("syscall" :
                 "=a" (return_value) :
                 "a" (SYSCALL_CREATEPROCESS), "D" (executable) :
                 "cc", "%r11", "%rcx");
 return return_value;
}

/*! Wrapper for the system call that pauses the process for a
 *  specified number of ticks.
 *  @param ticks integer holding the number of ticks the process should
 *         be paused.
 */
static inline long
pause(const int ticks)
{
 long return_value;
 __asm volatile("syscall" :
                 "=a" (return_value) :
                 "a" (SYSCALL_PAUSE), "D" (ticks) :
                 "cc", "%r11", "%rcx");
 return return_value;
}

/*! Wrapper for the system call that returns the current system time
 *  in ticks.
 */
static inline long
time(void)
{
 long return_value;
 __asm volatile("syscall" :
                 "=a" (return_value) :
                 "a" (SYSCALL_TIME) :
                 "cc", "%rcx", "%r11");
 return return_value;
}

/*! Wrapper for the system call that creates a thread. */
static inline long
createthread(void (*thread)(void), void* stack)
{
 long return_value;
 __asm volatile("syscall" :
                 "=a" (return_value) :
                 "a" (SYSCALL_CREATETHREAD), "D" (thread), "S" (stack) :
                 "cc", "%r11", "%rcx", "memory");
 return return_value;
}

/*! Wrapper for the system call that creates a semaphore. */
static inline long
createsemaphore(long initial_count)
{
 long return_value;
 __asm volatile("syscall" :
                 "=a" (return_value) :
                 "a" (SYSCALL_CREATESEMAPHORE), "D" (initial_count) :
                 "cc", "%rcx", "%r11", "memory");
 return return_value;
}

/*! Wrapper for the system call that performs a down operation on a
    semaphore. */
static inline long
semaphoredown(long semaphore_handle)
{
 long return_value;
 __asm volatile("syscall" :
                 "=a" (return_value) :
                 "a" (SYSCALL_SEMAPHOREDOWN), "D" (semaphore_handle) :
                 "cc", "%r11", "%rcx", "memory");
 return return_value;
}

/*! Wrapper for the system call that performs an up operation on a
    semaphore */
static inline long
semaphoreup(long semaphore_handle)
{
 long return_value;
 __asm volatile("syscall" :
                 "=a" (return_value) :
                 "a" (SYSCALL_SEMAPHOREUP), "D" (semaphore_handle) :
                 "cc", "%r11", "%rcx", "memory");
 return return_value;
}

#endif
