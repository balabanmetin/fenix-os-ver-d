/* Copyright (c) 1997-2012, FenixOS Developers
   All Rights Reserved.

   This file is subject to the terms and conditions defined in
   file 'LICENSE', which is part of this source code package.
 */

/*! \file Globals.hpp This file holds declarations of various global
    variables.
 */

#ifndef GLOBALS_H
#define GLOBALS_H

# include <stdint.h>

# include "instruction_wrappers.h"

# include <sysdefines.h>


/*! Size of the meta-data of dynamically allocated block in bytes. */
#define BLOCKSIZE 32

/*! Size of the memory that can be used for dynamic allocation. I refrain
 *  to overflow it.
 */
#define ALLOCATE_SIZE    (((amd64_top_of_available_physical_memory-amd64_lowest_available_physical_memory))<<6)

/*! States of a process. Currently, they are not used. They will be used when threads
 * are implemented or scheduling algorithm is changed.
 */
#define READY 0
#define RUNNING 1
#define BLOCKED 2

/*! Size of a cache line in bytes. The size corresponds to the size of a
    coherent cache line and can be larger than the level one cache line.
    It usually corresponds to the size of a L2 cache line. */
#define AMD64_CACHE_LINE_SIZE 64

/*! Structure which stores a context. */
struct AMD64Context
{
 /* The following fields must be the first in the struct. The order of
    fields must not change or the assembly code will break. */
 uint8_t  fp_context[512] __attribute__((aligned (16))); // offset 0x0
 uint64_t rax;             // offset 0x200
 uint64_t rbx;             // offset 0x208
 uint64_t rcx;             // offset 0x210
 uint64_t rdx;             // offset 0x218
 uint64_t rdi;             // offset 0x220
 uint64_t rsi;             // offset 0x228
 uint64_t rbp;             // offset 0x230
 uint64_t rsp;             // offset 0x238
 uint64_t r8;              // offset 0x240
 uint64_t r9;              // offset 0x248
 uint64_t r10;             // offset 0x250
 uint64_t r11;             // offset 0x258
 uint64_t r12;             // offset 0x260
 uint64_t r13;             // offset 0x268
 uint64_t r14;             // offset 0x270
 uint64_t r15;             // offset 0x278
 uint64_t rip;             // offset 0x280
 uint64_t rflags;          // offset 0x288
 // Additional fields can be added below
 int      interrupt_context; // True iff the context is saved in a
                             // intterrupt handler
};

/*! Each processor has its own structure of this type. It is used
    to store data which is private to each cpu. */
struct AMD64KernelGSData
{
 /*! The currently executing context. */
 struct AMD64Context * volatile activeContext;    // offset 0

 /*! Scratchpad variable used by the context switch routines. */
 uint64_t                       scratchPad1;      // offset 8

 /*! The stack used in system call handlers. */
 uint64_t                       syscallStack;     // offset 16

 /*! The index of this processor. */
 uint64_t                       processorIndex;   // offset 24

 /*! The APIC id of the APIC connected to this processor. */
 uint64_t                       APICId;           // offset 28
};

/* ELF image structures. The names from the ELF64 specification are used
   and the structs are derived from the ELF64 specification. */

#    define EI_MAG0       0  /*!< The first four bytes in an ELF image is
                                   0x7f 'E' 'L' 'F'. */
#    define EI_MAG1       1
#    define EI_MAG2       2
#    define EI_MAG3       3
#    define EI_CLASS      4  /*!< The specific ELF image class. */
#    define EI_DATA       5  /*!< Describes if the image is big- or little-
                                  endian. */
#    define EI_VERSION    6  /*!< The version of the ELF specification the
                                  image adheres to. */
#    define EI_OSABI      7  /*!< Type of OS the image can be run on. */
#    define EI_ABIVERSION 8  /*!< Version of the ABI used. */
#    define EI_PAD        9  /*!< First unused byte in the identification
                                  array. */
#    define EI_NIDENT     16 /*!< Number of entries in the identification
                                  array */

/*! Defines an ELF64 file header. */
struct Elf64_Ehdr
{
 uint8_t  e_ident[EI_NIDENT]; /*!< Array of bytes that shows that
                                        this is an ELF image and what type
                                        of ELF image it is. */
 uint16_t e_type;             /*!< The type of ELF executable image. */
 uint16_t e_machine;          /*!< Identifies the type of machine
                                   that the image can execute on. */
 uint32_t e_version;          /*!< The version of the ELF
                                   specification the image adheres to. */
 uint64_t e_entry;            /*!< Start address of the executable. */
 uint64_t e_phoff;            /*!< The offset into the image where
                                   the program header table is
                                   found. */
 uint64_t e_shoff;            /*!< The offset into the image where
                                   the program header table is
                                   found. */
 uint32_t e_flags;            /*!< Flags that are machine specific.
                                   These can be used to differentiate
                                   between similar machines. */
 uint16_t e_ehsize;           /*!< The size, in bytes, of the header. */
 uint16_t e_phentsize;        /*!< The size, in bytes, of each entry
                                   in the program header table. */
 uint16_t e_phnum;            /*!< The size, in entries, of the
                                   program header table. */
 uint16_t e_shentsize;        /*!< The size, in bytes, of each entry
                                   in the section header table. */
 uint16_t e_shnum;            /*!< The size, in entries, of the
                                   section header table. */
 uint16_t e_shstrndx;         /*!< The index, into the section table,
                                   of the section name string table.*/
};

/*! Defines an entry in the ELF program header table. Each entry
    corresponds to a segment. */
struct Elf64_Phdr
{
 uint32_t p_type;   /*!< Segments can have several types. p_type holds the
                         type. */
 uint32_t p_flags;  /*!< The attribute flags of the segment. */
 uint64_t p_offset; /*!< Offset into the image of the first byte of the
                         segment */
 uint64_t p_vaddr;  /*!< The (virtual) address to which the segment is to
                         be loaded. */
 uint64_t p_paddr;  /*!< Not used. */
 uint64_t p_filesz; /*!< The number of bytes the segment occupies in the
                         image. */
 uint64_t p_memsz;  /*!< The number of bytes the segment occupies in
                         memory. */
 uint64_t p_align;  /*!< The alignment the segment should have in memory.
                         This field is currently being ignored. */
};

/* Values used in p_type */
#    define PT_NULL 0 /*!< The entry is not used. */
#    define PT_LOAD 1 /*!< The segment can be loaded into memory. */
#    define PT_PHDR 6 /*!< The segment only hold a program header table. */
     /* Values used in p_flags */
#    define PF_X        0x1        /*!< Segment can be executed.*/
#    define PF_W        0x2        /*!< Segment can be written. */
#    define PF_R        0x4        /*!< Segment can be read. */
#    define PF_MASKPERM 0x0000FFFF /*!< Used to mask the permission bits */

/*! Copies an ELF image to memory. Does some checks to avoid that corrupt
    images gets copied to memory.
    \return The address to the first instruction to execute. */
extern void copy_ELF(const struct Elf64_Ehdr* const elf_image, uint64_t * entry_point, uint64_t * memory_block);

/*! Grabs a spin lock with write permissions */
inline void
grab_lock_rw(register volatile unsigned  int * const spin_lock
             /*!< Points to the spin lock. */)
{
 while (1)
 {
  while (*spin_lock !=0);

  if (0 == lock_cmpxchg32(spin_lock, 0, 0x80000000))
  return;
 }
}

/*! Grabs a spin lock with read permissions. */
inline void
grab_lock_r(register volatile unsigned int * const spin_lock
            /*!< Points to the spin lock. */)
{
 while (1)
 {
  register unsigned int spin_lock_value, new_spin_lock_value;

  while (((*spin_lock) & 0x80000000) !=0);

  new_spin_lock_value = *spin_lock;

  while (1)
  {
   spin_lock_value = new_spin_lock_value;

   if (spin_lock_value > 0x7ffffffe)
    break;

   new_spin_lock_value = lock_cmpxchg32(spin_lock,
                                        spin_lock_value,
                                        spin_lock_value+1);
   if (new_spin_lock_value == spin_lock_value)
    return;
  }
 }
}

/*! Releases a spin lock. */
inline void
release_lock(register volatile unsigned int * const spin_lock
             /*!< Points to the spin lock. */)
{
 register unsigned int spin_lock_value, new_spin_lock_value;

 new_spin_lock_value = *spin_lock;

 while (1)
 {
  spin_lock_value = new_spin_lock_value;

  if (0x80000000 == spin_lock_value)
   new_spin_lock_value = 0;
  else
   new_spin_lock_value = spin_lock_value-1;

  new_spin_lock_value = lock_cmpxchg32(spin_lock,
                                       spin_lock_value,
                                       new_spin_lock_value);
  if (new_spin_lock_value == spin_lock_value)
   return;
 }
}

/*! Returns the currently active context. */
inline struct AMD64Context *
getActiveContext(void)
{
 register struct AMD64Context * context;
 __asm volatile("mov %%gs:0,%0" : "=r" (context) : : );
 return context;
}

/*! Set the currently active context. */
inline void
setActiveContext(struct AMD64Context * const context)
{
 __asm volatile("mov %0,%%gs:0" : : "r" (context) : );
}

/*! Get the index for the CPU.
    \returns The index into the amd64_CPU_private_table for the current
             CPU. */
inline const int
get_processor_index(void)
{
 register int current_processor;

 __asm volatile ("mov %%gs:24,%0" : "=r"(current_processor));

 return current_processor;
}

/* Performs a context switch to user space or waits for interrupts if
   there is no valid context. Will return to caller if the context
   is not valid. */
inline void
returnToUserLevel(const struct AMD64Context * const context,
                  const int from_interrupt)
{
 /* This is a hack. This code will perform a decent context switch but
    more state should be switched. There are also performance
    optimizations to be made. */

 if (0 == context)
 {
  /* No proper context and called from interrupt. Must return. */
  if (from_interrupt)
   return;

  /* No proper context. Halt and wait for the next interrupt, then return.
   */
  sti();
  hlt();
  cli();
  return;
 }

 /* Set FPU/MMX/SSE context. */
 fxrstor(context->fp_context);

 /* swap the gs. */
 __asm volatile("swapgs" :::);

 if (context->interrupt_context)
 {
  /* Set registers and return via iretq */
  __asm volatile("movq    0x200(%%rcx),%%rax \n \
                  movq    0x208(%%rcx),%%rbx \n \
                  movq    0x220(%%rcx),%%rdi \n \
                  movq    0x228(%%rcx),%%rsi \n \
                  movq    0x230(%%rcx),%%rbp \n \
                  movq    0x240(%%rcx),%%r8 \n \
                  movq    0x248(%%rcx),%%r9 \n \
                  movq    0x250(%%rcx),%%r10 \n \
                  movq    0x258(%%rcx),%%r11 \n \
                  movq    0x260(%%rcx),%%r12 \n \
                  movq    0x268(%%rcx),%%r13 \n \
                  movq    0x270(%%rcx),%%r14 \n \
                  movq    0x278(%%rcx),%%r15 \n \
                  movq    $11,%%rdx \n \
                  pushq   %%rdx \n \
                  pushq   0x238(%%rcx) \n \
                  pushq   0x288(%%rcx) \n \
                  pushq   $19 \n \
                  pushq   0x280(%%rcx) \n \
                  pushq   0x210(%%rcx) \n \
                  pushq   0x218(%%rcx) \n \
                  mov     %%dx,%%gs \n \
                  mov     %%dx,%%fs \n \
                  mov     %%dx,%%es \n \
                  mov     %%dx,%%ds \n \
                  popq    %%rdx \n \
                  popq    %%rcx \n \
                  iretq"
                 :
                 : "c"(context)
                 : "memory");
 }
 else
 {
  /* Set registers and return via sysret */
  __asm volatile("movq    0x200(%%rcx),%%rax \n \
                  movq    0x208(%%rcx),%%rbx \n \
                  movq    0x218(%%rcx),%%rdx \n \
                  movq    0x220(%%rcx),%%rdi \n \
                  movq    0x228(%%rcx),%%rsi \n \
                  movq    0x230(%%rcx),%%rbp \n \
                  movq    0x240(%%rcx),%%r8 \n \
                  movq    0x248(%%rcx),%%r9 \n \
                  movq    0x250(%%rcx),%%r10 \n \
                  movq    0x260(%%rcx),%%r12 \n \
                  movq    0x268(%%rcx),%%r13 \n \
                  movq    0x270(%%rcx),%%r14 \n \
                  movq    0x278(%%rcx),%%r15 \n \
                  movq    0x288(%%rcx),%%r11 \n \
                  pushq   0x238(%%rcx) \n \
                  pushq   0x280(%%rcx) \n \
                  mov     $11,%%ecx \n \
                  mov     %%cx,%%gs \n \
                  mov     %%cx,%%fs \n \
                  mov     %%cx,%%es \n \
                  mov     %%cx,%%ds \n \
                  popq    %%rcx \n \
                  popq    %%rsp \n \
                  sysretq"
                 :
                 : "c"(context)
                 : "memory");
 }
}

/*! Array holding the private data for all processors. */
extern struct AMD64KernelGSData
amd64_CPU_private_table[16];

extern void
amd64_syscall_entry_point(void) __attribute__ ((noreturn));

extern void
amd64_syscall_dummy_target(void) __attribute__ ((noreturn));

extern void
amd64_enter_kernel(void) __attribute__ ((noreturn));

extern void
amd64_handle_interrupt_user(int interrupt) __attribute__ ((noreturn));

extern void
amd64_handle_interrupt_super(int interrupt);

extern void
amd64_handle_exception_user(int exc) __attribute__ ((noreturn));

extern void
amd64_handle_exception_super(int exc);

extern void
amd64_handle_exception_w_error_user(int exc,
                                    uint32_t code) __attribute__ ((noreturn));

extern void
amd64_handle_exception_w_error_super(int exc, uint32_t code);

extern void
amd64_handle_nmi_user(int dummy) __attribute__ ((noreturn));

extern void
amd64_handle_nmi_super(int dummy);

extern const struct Elf64_Ehdr* const 
ELF_images[1];
/*!< Array of pointers to ELF images. The array ends with a null
     pointer. */

/*! Assembly routine which will halt the calling processor forever. */
extern void
amd64_halt(void)  __attribute__ ((noreturn));

/*! Assembly routine which will cause the calling processor to envoke
    the Bochs debugger. */
extern void
amd64_enter_debugger(void);

/*! Lowest address of available physical memory. */
extern uint64_t
amd64_lowest_available_physical_memory;

/*! Highest address of available physical memory. */
extern uint64_t
amd64_top_of_available_physical_memory;

/*! Base address of the local APIC. */
extern volatile uint32_t *
amd64_local_APIC_base_address;

inline void
send_IPI(register unsigned char const destination_processor_index,
         register unsigned int  const vector)
{
 /* Set destination. */
 *(amd64_local_APIC_base_address+0x310/4) =
 amd64_CPU_private_table[destination_processor_index].APICId<<(56-32);
 /* And send interrupts. */
 *(amd64_local_APIC_base_address+0x300/4) = vector & 0xff;
}
//////////////////////////////////

/*! Meta-data of allocated block. Each dynamically allocated block is
 * preceded by that data structure.
 */

typedef struct block * blockPtr;

 struct block {
  uint32_t full; /*!< Indicator of emptiness of cell. 0 if empty, 1 otherwise. */
  uint64_t size; /*!< Size of the allocated memory block corresponding that strucure. */
  blockPtr prev; /*!< Pointer to the previous memory block's meta-data.*/
  blockPtr next; /*!< Pointer to the next memory block's meta-data.*/
};

/*! Allocates a memory block.
    \return an address to the memory block or an error code if
            the allocation was not successful. */
extern long
kalloc(const register uint64_t length
       /*!< The number of bytes to allocate. */);

/*! Frees a previously allocated a memory block.
    \return ALL_OK if successful or an error code if
            the free was not successful. */
extern long
kfree(const register uint64_t address
       /*!< The address to the memory block to free. */);


/*! Terminates the caller process. */
extern void kterminate();

/*! Creates a process and pushes it to the process queue.
 * \return status of operation. */

extern unsigned long kcreateprocess(uint64_t rdi
		/*< The array index of elf image*/);

/*! scheduler function is called when PIT interrupt (interrupt 32) interrupts
 * the operating system. The frequency of scheduler calls (25 Hz) is one eighth of PIT
 * interrupt frequency (200 Hz) .*/
extern void scheduler();


/*! Each process is described with one this data structure.*/
struct process_entry {
	uint64_t                   id; /*!<Index of elf image in elf images array */
	uint64_t      memory_location; /*!< The initial memory location of program's segments */
	struct AMD64Context * context; /*!<A pointer to the context associated with the program*/
	uint64_t                state; /*!< State indicator. READY, RUNNING and BLOCKED are three options */

	//*state will come here
	//*threads will come here. May be a semaphore
};

/*! Process queue linked list unit. */
struct process_queue_element {
	struct process_entry        element; /*!< The process entry */
	struct process_queue_element * next; /*!<Pointer to the next element */
};

/*! Pointer to the top element in queue. This process is executed by CPU */
extern struct process_queue_element *  top_process;

/*! Pointer to the last element in queue. This process is executed by CPU */
extern struct process_queue_element * back_process;

/*! queue emptiness check.
 * \return 1 if queue is empty. 0 otherwise.
 */
extern uint64_t is_empty_process_queue();

/*! Pushes the process to the queue end. */
extern void push_back_process_queue(struct process_entry
		/*!<Process to be pushed */);

/*! \return process struct of the top-most element in queue. */
extern struct process_entry top_process_queue();

/*! Removes the top-most element of the queue.
 * \return process struct of the top-most element in queue.
 */
extern struct process_entry pop_process_queue();

/*! Outputs a string through the terminal emulator. */
extern void
kprints(const char* const string
        /*!< Points to a null terminated string */);

/*! Outputs an unsigned 64-bit value through the terminal emulator. */
extern void
kprinthex(const register uint64_t value /*! value to be printed. */);

#endif
