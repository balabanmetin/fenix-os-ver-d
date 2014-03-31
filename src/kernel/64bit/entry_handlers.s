/* Copyright (c) 1997-2012, FenixOS Developers
   All Rights Reserved.

   This file is subject to the terms and conditions defined in
   file 'LICENSE', which is part of this source code package.
 */

/**
 * \file entry_handlers.s This file contains system call entry points.
 *       These routines pass control to the C++ code.
 */

 .global amd64_interrupt_entries
 .global amd64_syscall_entry_point
 .global amd64_syscall_dummy_target
 .global amd64_halt
 .global amd64_enter_debugger

 .text

 # Build interrupt handlers for all 256 interrupt vectors

 .align 16
/**
 * Array with interrupt entry points. 
 */
amd64_interrupt_entries:
 .set interrupt_number, 0
 .rept 2
 pushq  $interrupt_number
 jmp    amd64_handle_exception
 .align 16
 .set interrupt_number, interrupt_number+1
 .endr

 .rept 1
 pushq  $interrupt_number
 jmp    amd64_handle_nmi
 .align 16
 .set interrupt_number, interrupt_number+1
 .endr

 .rept 5
 pushq  $interrupt_number
 jmp    amd64_handle_exception
 .align 16
 .set interrupt_number, interrupt_number+1
 .endr

 .rept 1
 pushq  $interrupt_number
 jmp    amd64_handle_double_fault
 .align 16
 .set interrupt_number, interrupt_number+1
 .endr

 .rept 1
 pushq  $interrupt_number
 jmp    amd64_handle_exception
 .align 16
 .set interrupt_number, interrupt_number+1
 .endr

 .rept 5
 pushq  $interrupt_number
 jmp    amd64_handle_exception_w_error
 .align 16
 .set interrupt_number, interrupt_number+1
 .endr

 .rept 2
 pushq  $interrupt_number
 jmp    amd64_handle_exception
 .align 16
 .set interrupt_number, interrupt_number+1
 .endr

 .rept 1
 pushq  $interrupt_number
 jmp    amd64_handle_exception_w_error
 .align 16
 .set interrupt_number, interrupt_number+1
 .endr

 .rept 14
 pushq  $interrupt_number
 jmp    amd64_handle_exception
 .align 16
 .set interrupt_number, interrupt_number+1
 .endr

 .rept 255-32
 pushq  $interrupt_number
 jmp    amd64_handle_interrupt
 .align 16
 .set interrupt_number, interrupt_number+1
 .endr

 # The last vector, vector 256

 pushq  $interrupt_number


 .macro save_context error_code, in_interrupt
 mov     %rax,0x200(%rbp)
 mov     %rbx,0x208(%rbp)
 .ifeq   \in_interrupt
 mov     %rcx,0x280(%rbp)
 .else
 mov     %rcx,0x210(%rbp)
 .endif
 mov     %rdx,0x218(%rbp)
 mov     %rdi,0x220(%rbp)
 mov     %rsi,0x228(%rbp)
 .ifeq   \in_interrupt
 mov     %rsp,0x238(%rbp)
 .endif
 mov     %r8,0x240(%rbp)
 mov     %r9,0x248(%rbp)
 mov     %r10,0x250(%rbp)
 .ifeq   \in_interrupt
 mov     %r11,0x288(%rbp)
 .else
 mov     %r11,0x258(%rbp)
 .endif
 mov     %r12,0x260(%rbp)
 mov     %r13,0x268(%rbp)
 mov     %r14,0x270(%rbp)
 mov     %r15,0x278(%rbp)
 .endm

 # Macro used for interrupt and system call entry routines
 .macro irq_macro error_code, function_called_if_in_user, function_called_if_in_kernel, is_interrupt
 # On the stack is:
 # 64: SS
 # 64: RSP
 # 64: RFLAGS
 # 64: CS (lowest 16 bits)
 # 64: RIP
 # 64: (error) (lowest 32 bits)
 # 64: interrupt number

 .if     \is_interrupt
 cmpw    $24,16+8*\error_code(%rsp) # Check the CS for priv level
 jz      interrupt_in_kernel\@
 .else
 .endif
 swapgs
 mov     %rbp,%gs:8            # save rbp into scratch_pad_1
 mov     %gs:0,%rbp            # rbp now points to the active context

 save_context \error_code,\is_interrupt

 mov     %gs:8,%rax           # rbp
 mov     %rax,0x230(%rbp)

 mov     $32,%eax
 .if     \is_interrupt
 mov     %ax,%ss
 .endif
 mov     %ax,%ds
 mov     %ax,%es
 mov     %ax,%fs

 .if     \is_interrupt
 popq    %rdi                  # Interrupt number
 .if     \error_code
 popq    %rsi
 .endif

 popq    0x280(%rbp)           # pop RIP
 popq    %rax
 popq    0x288(%rbp)           # pop RFLAGS
 popq    0x238(%rbp)           # pop RSP

 .else
 mov     %gs:16,%rsp      # set the system call stack
 .endif

 # Call C++ part. We will never return.
 call   \function_called_if_in_user
 jmp    amd64_halt

 .if     \is_interrupt
interrupt_in_kernel\@:
 pushq   %rax                # Push all registers. This can be optimized by
                             # just storing the registers not saved by the
                             # callee!
 pushq   %rbx
 pushq   %rcx
 pushq   %rdx
 pushq   %rsi
 pushq   %rdi
 pushq   %rbp
 pushq   %r8
 pushq   %r9
 pushq   %r10
 pushq   %r11
 pushq   %r12
 pushq   %r13
 pushq   %r14
 pushq   %r15

 # call c part
 mov     15*8(%rsp),%rdi    # Error code
 .if     \error_code
 mov     16*8(%rsp),%rsi    # Error code
 .endif
 call    \function_called_if_in_kernel

 popq    %r15
 popq    %r14
 popq    %r13
 popq    %r12
 popq    %r11
 popq    %r10
 popq    %r9
 popq    %r8
 popq    %rbp
 popq    %rdi
 popq    %rsi
 popq    %rdx
 popq    %rcx
 popq    %rbx
 popq    %rax
 add     $8+8*\error_code,%rsp  # skip irq number and error code
 iretq
 .endif
 .endm

amd64_handle_interrupt:
 irq_macro 0,amd64_handle_interrupt_user,amd64_handle_interrupt_super,1

amd64_handle_exception_w_error:
 irq_macro 1,amd64_handle_exception_w_error_user,amd64_handle_exception_w_error_super,1

amd64_handle_exception:
 irq_macro 0,amd64_handle_exception_user,amd64_handle_exception_super,1

amd64_handle_nmi:
 irq_macro 0,amd64_handle_nmi_user,amd64_handle_nmi_super,1

/**
 *  The entry point for syscall invoked system calls.
 */
 .align  16
amd64_syscall_entry_point:
 irq_macro 0,amd64_enter_kernel,0,0

 .align  16
/** Dummy entry point for system calls. Using this entry point will cause
 *  the Bochs debugger to be invoked.
 */
amd64_syscall_dummy_target:

/** 
 *  Invokes the Bochs debugger. 
 */
amd64_enter_debugger:
 push  %rdx
 push  %rax

 mov   $0x8a00,%rdx
 mov   %rdx,%rax
 outw  %ax,%dx
 mov   $0x8ae0,%rax
 outw  %ax,%dx

 popq  %rax
 popq  %rdx
 jmp   amd64_enter_debugger

amd64_handle_double_fault:
 call  amd64_enter_debugger
 jmp   amd64_halt

/** 
 *  Halts the calling CPU. 
 */
amd64_halt:
 hlt
 jmp   amd64_halt

 .end
