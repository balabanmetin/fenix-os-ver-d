/* Copyright (c) 1997-2012, FenixOS Developers
   All Rights Reserved.

   This file is subject to the terms and conditions defined in
   file 'LICENSE', which is part of this source code package.
 */

/*! \file instruction_wrappers.h This file holds wrappers for various
   assembly language snippets. These wrappers allow access to low-level
   features such as machine specific registers and instructions that support
   synchronization primitives. */

#ifndef instruction_wrappers_H
#define instruction_wrappers_H

# include <stdint.h>

/*! Wrapper for the hlt instruction. */
inline void
hlt(void)
{
 __asm volatile("hlt" : : : );
}


/*! Wrapper for the cpuid instruction. */
inline void
cpuid(register const uint32_t functionNumber /*!< The number of the
                                                  requested function.
                                              */,
       register uint32_t *    EAXValue       /*!< The value returned
                                                   in the EAX register.
                                               */,
       register uint32_t *    EBXValue       /*!< The value returned
                                                   in the EBX register.
                                               */,
       register uint32_t *    ECXValue       /*!< The value returned
                                                   in the ECX register.
                                               */,
       register uint32_t *    EDXValue       /*!< The value returned
                                                   in the EDX register.
                                               */)
{
 __asm volatile("cpuid" :
                "=a" (*EAXValue),
                "=b" (*EBXValue),
                "=c" (*ECXValue),
                "=d" (*EDXValue) :
                "a" (functionNumber) : );
}

/*! Wrapper for the rdmsr instruction. */
inline void
rdmsrl(register const uint32_t MSR      /*!< The number of the MSR to
                                             read. */,
       register uint32_t *     EAXValue /*!< The low-order 32 bits of the
                                             MSR. */,
       register uint32_t *     EDXValue /*!< The high-order 32 bits of the
                                             MSR. */)
{
 __asm volatile("rdmsr" :
                "=a" (*EAXValue),
                "=d" (*EDXValue) :
                "c" (MSR) : );
}

/*! Wrapper for the rdmsr instruction.
    \returns The value read from the MSR. */
inline uint64_t
rdmsr(register const uint32_t MSR  /*!< The number of the MSR to read.*/)
{
 uint32_t eax, edx;

 rdmsrl(MSR, &eax, &edx);

 return ((uint64_t) edx<<32) | (uint64_t) eax;
}


/*! Wrapper for the wrmsr instruction. */
inline void
wrmsrl(register const uint32_t MSR      /*!< The number of the MSR to
                                             write. */,
       register const uint32_t EAXValue /*!< The low-order 32 bits of the
                                             value to write to the MSR. */,
       register const uint32_t EDXValue /*!< The high-order 32 bits of the
                                             value to write to the MSR. */)
{
 __asm volatile("wrmsr" : :
                "a" (EAXValue),
                "d" (EDXValue),
                "c" (MSR) : );
}

/*! Wrapper for the wrmsr instruction. */
inline void
wrmsr(register const uint32_t  MSR      /*!< The number of the MSR to
                                             write. */,
      register const uint64_t value     /*!< The value to write to the
                                             MSR. */)
{
 wrmsrl(MSR, value & 0xffffffff, (value>>32) & 0xffffffff);
}


/*! Wrapper for reading the cr0 register.
    \returns The value in the cr0 register. */
inline uint64_t
readCr0(void)
{
 register uint64_t returnValue;
 __asm volatile("movq %%cr0,%0" :
                "=r" (returnValue) : : );
 return returnValue;
}

/*! Wrapper for reading the cr2 register.
    \returns The value in the cr2 register. */
inline uint64_t
readCr2(void)
{
 register uint64_t returnValue;
 __asm volatile("movq %%cr2,%0" :
                "=r" (returnValue) : : );
 return returnValue;
}

/*! Wrapper for reading the cr3 register.
    \returns The value in the cr3 register. */
inline uint64_t
readCr3(void)
{
 register uint64_t returnValue;
 __asm volatile("movq %%cr3,%0" :
                "=r" (returnValue) : : );
 return returnValue;
}

/*! Wrapper for reading the cr4 register.
    \returns The value in the cr4 register. */
inline uint64_t
readCr4(void)
{
 register uint64_t returnValue;
 __asm volatile("movq %%cr4,%0" :
                "=r" (returnValue) : : );
 return returnValue;
}

/*! Wrapper for reading the cr8 register.
    \returns The value in the cr8 register. */
inline uint64_t
readCr8(void)
{
 register uint64_t returnValue;
 __asm volatile("movq %%cr8,%0" :
                "=r" (returnValue) : : );
 return returnValue;
}

/*! Wrapper for writing the cr0 register. */
inline void
writeCr0(register const uint64_t value /*!< The value to write to the
                                            register. */)
{
 __asm volatile("movq %0,%%cr0" : :
                "r" (value) : );
}

/*! Wrapper for writing the cr3 register. */
inline void
writeCr3(register const uint64_t value /*!< The value to write to the
                                            register. */)
{
 __asm volatile("movq %0,%%cr3" : :
                "r" (value) : );
}

/*! Wrapper for writing the cr4 register. */
inline void
writeCr4(register const uint64_t value /*!< The value to write to the
                                            register. */)
{
 __asm volatile("movq %0,%%cr4" : :
                "r" (value) : );
}

/*! Wrapper for writing the cr8 register. */
inline void
writeCr8(register const uint64_t value /*!< The value to write to the
                                            register. */)
{
 __asm volatile("movq %0,%%cr8" : :
                "r" (value) : );
}

/*! Wrapper for reading the dr6 register.
    \returns The value in the dr6 register. */
inline uint64_t
readDr6(void)
{
 register uint64_t returnValue;
 __asm volatile("movq %%dr6,%0" :
                "=r" (returnValue) : : );
 return returnValue;
}

/*! Wrapper for writing the dr0 register. */
inline void
writeDr0(register const uint64_t value /*!< The value to write to the
                                            register. */)
{
 __asm volatile("movq %0,%%dr0" : :
                "r" (value) : );
}

/*! Wrapper for writing the dr1 register. */
inline void
writeDr1(register const uint64_t value /*!< The value to write to the
                                            register. */)
{
 __asm volatile("movq %0,%%dr1" : :
                "r" (value) : );
}

/*! Wrapper for writing the dr2 register. */
inline void
writeDr2(register const uint64_t value /*!< The value to write to the
                                            register. */)
{
 __asm volatile("movq %0,%%dr2" : :
                "r" (value) : );
}

/*! Wrapper for writing the dr3 register. */
inline void
writeDr3(register const uint64_t value /*!< The value to write to the
                                            register. */)
{
 __asm volatile("movq %0,%%dr3" : :
                "r" (value) : );
}

/*! Wrapper for writing the dr6 register. */
inline void
writeDr6(register const uint64_t value /*!< The value to write to the
                                            register. */)
{
 __asm volatile("movq %0,%%dr6" : :
                "r" (value) : );
}

/*! Wrapper for writing the dr7 register. */
inline void
writeDr7(register const uint64_t value /*!< The value to write to the
                                            register. */)
{
 __asm volatile("movq %0,%%dr7" : :
                "r" (value) : );
}

/*! Wrapper for the cli instruction. */
inline void
cli(void)
{
 __asm volatile("cli" : : : );
}

/*! Wrapper for the invd instruction. */
inline void
invdp(void)
{
 __asm volatile("invd" : : : "memory" );
}

/*! Wrapper for the invlpg instruction.*/
inline void
invlpg(register const uint64_t virtualAddress /*!< The invlpg instruction
                                                   invalidates the TLB
                                                   entries corresponding
                                                   to the virtual address
                                                   passed as
                                                   virtualAddress. */ )
{
 __asm volatile("invlpg (%%rax)" : : "a" (virtualAddress) : "memory" );
}

/*! Wrapper for the lgdt instruction. */
inline void
lgdt(register const uint16_t limit       /*!< The limit of the GDT. This
                                              is the size of the GDT minus
                                              one byte. */,
     register const uint64_t  GDTAddress /*!< The starting address of the
                                              GDT. */)
{
 __asm volatile("sub $16,%%rsp\n mov %%rax,8(%%rsp)\n \
                 mov %%bx,6(%%rsp)\n lgdt 6(%%rsp)\n add $16,%%rsp" :
                : "a" (GDTAddress), "b" (limit) : "memory");
}

/*! Wrapper for the lidt instruction. */
inline void
lidt(register const uint16_t limit       /*!< The limit of the IDT. This
                                              is the size of the IDT minus
                                              one byte. */,
     register const uint64_t  IDTAddress /*!< The starting address of
                                              the IDT. */)
{
 __asm volatile("sub $16,%%rsp\n mov %%rax,8(%%rsp)\n \
                 mov %%bx,6(%%rsp)\n lidt 6(%%rsp)\n add $16,%%rsp" :
                 : "a" (IDTAddress), "b" (limit) : );
}

/*! Wrapper for the lldt instruction. */
inline void
lldt(register const uint16_t selector /*!< A selector into the GDT for the
                                           LDT. */)
{
 __asm volatile("lldt %%ax" : : "a" (selector) : "memory");
}

/*! Wrapper for the ltr instruction. */
inline void
ltr(register const uint16_t selector  /*!< A selector into the GDT
                                           for the TSS. */)
{
 __asm volatile("ltr %%ax" : : "a" (selector) : );
}

/*! Wrapper for the sti instruction. */
inline void
sti(void)
{
 __asm volatile("sti" : : : );
}

/*! Wrapper for the wbinvd instruction. */
inline void
wbinvd(void)
{
 __asm volatile("wbinvd" : : : "memory" );
}

/*! Wrapper for a 8-bit out instruction. */
inline void
out8(register const int16_t portNumber  /*!< The number of the port to
                                             write to. */,
     register const int8_t  outputValue /*!< The value to be written to the
                                             port. */)
{
 __asm volatile("outb %%al,%%dx" : : "d" (portNumber), "a" (outputValue));
}

/*! Wrapper for a 16-bit out instruction. */
inline void
out16(register const int16_t portNumber  /*!< The number of the port to
                                              write to. */,
      register const int16_t outputValue /*!< The value to be written to the
                                              port. */)
{
 __asm volatile("outw %%ax,%%dx" : : "d" (portNumber), "a" (outputValue));
}

/*! Wrapper for a 32-bit out instruction. */
inline void
out32(register const int16_t portNumber  /*!< The number of the port to
                                              write to. */,
      register const int32_t outputValue /*!< The value to be written to the
                                              port. */)
{  
 __asm volatile("outl %%eax,%%dx" :: "d" (portNumber), "a" (outputValue));
}

/*! Wrapper for a 8-bit in instruction.
    \returns The value read. */
inline int8_t
inInt8(register const int16_t  portNumber /*! The number of the port to
                                              read from. */ )
{
 register int8_t returnValue;
 __asm volatile("inb %%dx,%%al" : "=a" (returnValue) : "d" (portNumber));
 return returnValue;
}

/*! Wrapper for a 16-bit in instruction.
    \returns The value read. */
inline int16_t
inInt16(register const int16_t portNumber /*! The number of the port to
                                              read from. */ )
{
 register short returnValue;
 __asm volatile("inw %%dx,%%ax" : "=a" (returnValue) : "d" (portNumber));
 return returnValue;
}

/*! Wrapper for a 32-bit in instruction.
    \returns The value read. */
inline int
inInt32(register const int16_t  portNumber /*! The number of the port to
                                               read from. */ )
{
 register int returnValue;
 __asm volatile("inl %%dx,%%eax" : "=a" (returnValue) : "d" (portNumber));
 return returnValue;
}

/*! Wrapper for a 64-bit locked xchg instruction.
    \returns The value stored in the variable pointed to by
             pointer_to_variable before the operation is performed. */
inline uint64_t
lock_xchg64(register volatile uint64_t * const pointer_to_variable
            /*!< Pointer to the variable to operate on. */,
            register uint64_t                  value
            /*!< The value to be written to memory. */)
{
 __asm volatile("lock xchgq %0,%1"
                : "=r" (value) 
                : "m" (*pointer_to_variable), "0" (value)
                : "memory");
 return value;
}

/*! Wrapper for a 32-bit locked xchg instruction.
    \returns The value stored in the variable pointed to by
             pointer_to_variable before the operation is performed. */
inline uint32_t
lock_xchg32(register volatile uint32_t * const pointer_to_variable 
            /*!< Pointer to the variable to operate on. */,
            register uint32_t                  value
            /*!< The value to be written to memory. */)
{
 __asm volatile("lock xchgl %k0,%1"
                : "=r" (value)
                : "m" (*pointer_to_variable), "0" (value)
                : "memory");
 return value;
}

/*! Wrapper for a 16-bit locked xchg instruction.
    \returns The value stored in the variable pointed to by
             pointer_to_variable before the operation is performed. */
inline uint16_t
lock_xchg16(register volatile uint16_t * const pointer_to_variable
            /*!< Pointer to the variable to operate on. */,
            register uint16_t                  value
            /*!< The value to be written to memory. */)
{
 __asm volatile("lock xchgw %w0,%1"
                : "=r" (value)
                : "m" (*pointer_to_variable), "0" (value)
                : "memory");
 return value;
}

/*! Wrapper for a 8-bit locked xchg instruction.
    \returns The value stored in the variable pointed to by
             pointer_to_variable before the operation is performed. */
inline uint8_t
lock_xchg8(register volatile uint8_t * const pointer_to_variable
           /*!< Pointer to the variable to operate on. */,
           register uint8_t                  value
           /*!< The value to be written to memory. */)
{
 __asm volatile("lock xchgb %b0,%1"
                : "=r" (value)
                : "m" (*pointer_to_variable), "0" (value)
                : "memory");
 return value;
}

/*! Wrapper for a 64-bit locked xadd instruction.
    \returns The value stored in the variable pointed to by
             pointer_to_variable before the operation is performed. */
inline uint64_t
lock_xadd64(register volatile uint64_t * const pointer_to_variable
            /*!< Pointer to the variable to operate on. */,
            register uint64_t                  increment
            /*!< Value to add to the variable. */)
{
 __asm volatile("lock xaddq %0,%1"
                : "=r" (increment) 
                : "m" (*pointer_to_variable), "0" (increment)
                : "memory");
 return increment;
}

/*! Wrapper for a 32-bit locked xadd instruction.
    \returns The value stored in the variable pointed to by
             pointer_to_variable before the operation is performed.  */
inline uint32_t
lock_xadd32(register volatile uint32_t * const pointer_to_variable
            /*!< Pointer to the variable to operate on. */,
            register uint32_t                  increment
            /*!< Value to add to the variable. */)
{
 __asm volatile("lock xaddl %k0,%1"
                : "=r" (increment) 
                : "m" (*pointer_to_variable), "0" (increment)
                : "memory");
 return increment;
}

/*! Wrapper for a 16-bit locked xadd instruction.
     \returns The value stored in the variable pointed to by
              pointer_to_variable before the operation is performed. */
inline uint16_t
lock_xadd16(register volatile uint16_t * const pointer_to_variable
            /*!< Pointer to the variable to operate on. */,
            register uint16_t                  increment
            /*!< Value to add to the variable. */)
{
 __asm volatile("lock xaddw %w0,%1"
                : "=r" (increment) 
                : "m" (*pointer_to_variable), "0" (increment)
                : "memory");
 return increment;
}

/*! Wrapper for a 8-bit locked xadd instruction.
    \returns The value stored in the variable pointed to by
             pointer_to_variable before the operation is performed. */
inline uint8_t
lock_xadd8(register volatile uint8_t * const pointer_to_variable
           /*!< Pointer to the variable to operate on. */,
           register uint8_t                  increment
           /*!< Value to add to the variable. */)
{
 __asm volatile("lock xaddb %b0,%1"
                : "=r" (increment) 
                : "m" (*pointer_to_variable), "0" (increment)
                : "memory");
 return increment;
}

/*! Wrapper for a 64-bit locked cmpxchg instruction.
    \returns The value stored in the variable pointed to by
             pointer_to_variable before the operation is performed. */
inline uint64_t
lock_cmpxchg64(register volatile uint64_t * const pointer_to_variable
               /*!< Pointer to the variable to operate on. */,
               register uint64_t                  old_value
               /*!< The value assumed to be in the variable. */,
               register const uint64_t            new_value
               /*!< The value to conditionally write to the variable. */)
{
 __asm volatile("lock cmpxchgq %1,%2"
                : "=a" (old_value)
                : "r"(new_value), "m" (*pointer_to_variable),
                  "0" (old_value)
                : "memory");

 return old_value;
}

/*! Wrapper for a 32-bit locked cmpxchg instruction.
    \returns The value stored in the variable pointed to by
             pointer_to_variable before the operation is performed. */
inline uint32_t
lock_cmpxchg32(register volatile uint32_t * const pointer_to_variable
               /*!< Pointer to the variable to operate on. */,
               register uint32_t                  old_value
               /*!< The value assumed to be in the variable. */,
               register const uint32_t            new_value
               /*!< The value to conditionally write to the variable. */)
{
 __asm volatile("lock cmpxchgl %k1,%2"
                : "=a" (old_value)
                : "r"(new_value), "m" (*pointer_to_variable),
                  "0" (old_value)
                : "memory");

 return old_value;
}

/*! Wrapper for a 16-bit locked cmpxchg instruction.
    \returns The value stored in the variable pointed to by
             pointer_to_variable before the operation is performed. */
inline uint16_t
lock_cmpxchg16(register volatile uint16_t * const pointer_to_variable
               /*!< Pointer to the variable to operate on. */,
               register uint16_t               old_value
               /*!< The value assumed to be in the variable. */,
               register const uint16_t         new_value
               /*!< The value to conditionally write to the variable. */)
{
 __asm volatile("lock cmpxchgw %w1,%2"
                : "=a" (old_value)
                : "r"(new_value), "m" (*pointer_to_variable),
                  "0" (old_value)
                : "memory");

 return old_value;
}

/*! Wrapper for a 8-bit locked cmpxchg instruction.
    \returns The value stored in the variable pointed to by
             pointer_to_variable before the operation is performed. */
inline uint8_t
lock_cmpxchg8(register volatile uint8_t * const pointer_to_variable
              /*!< Pointer to the variable to operate on. */,
              register uint8_t                  old_value
              /*!< The value assumed to be in the variable. */,
              register const uint8_t            new_value
              /*!< The value to conditionally write to the variable. */)
{
 __asm volatile("lock cmpxchgb %b1,%2"
                : "=a" (old_value) 
                : "r"(new_value), "m" (*pointer_to_variable), 
                "0" (old_value)
                : "memory");

 return old_value;
}

/*! Wrapper for the fxsave instruction. */
inline void
fxsave(register unsigned char * const context 
       /*!< Pointer to a 512-byte buffer in which the context is to be
            stored. */)
{
 __asm volatile("fxsaveq %0"
                : "=m" (*context)
                : 
                : "memory");
}

/*! Wrapper for the fxrstor instruction. */
inline void
fxrstor(register const unsigned char * const context
        /*!< Pointer to a 512-byte buffer from which the context is to be
             restored. */)
{
 __asm volatile("fxrstorq %0"
                :
                : "m" (*context)
                : "memory");
}

#endif
