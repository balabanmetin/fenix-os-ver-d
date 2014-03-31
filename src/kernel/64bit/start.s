/* Copyright (c) 1997-2012, FenixOS Developers
   All Rights Reserved.

   This file is subject to the terms and conditions defined in
   file 'LICENSE', which is part of this source code package.
 */

/**
 * \file start.s This file contains the entry code for the 64-bit kernel.
 */

 .text
 .global _start

/*
 * This is a small data structure used by the 32-bit kernel to get acces to
 * data structures in the 64-bit kernel.
 */
 .align 8
_amd64_kernel_data_structure:
 .ascii "Fenx"
 .int   0x10000
 .quad  amd64_lowest_available_physical_memory
 .quad  amd64_top_of_available_physical_memory
 .quad  amd64_BSP_GDT
 .quad  16*16+7*8
 .quad  amd64_pic_interrupt_map
 .quad  amd64_CPU_private_table
 .quad  amd64_local_APIC_base_address
 .quad  amd64_io_apic_address
 .quad  amd64_number_of_available_CPUs
	
/**
 * Entry point for the 64-bit kernel.
 */
_start:
 # Set a new stack
 movq  $0x200000,%rsp

 # Clear a page worth of memory so that the stack has a defined state. This is
 # a little bit of overkill.
 mov     $512,%rsi
 xor     %rcx,%rcx
clear_stack_loop:
 push    %rcx
 dec     %rsi
 jnz     clear_stack_loop

 # adjust the stack back
 add     $4096,%rsp
	
 # Set all flags to a well defined state
 push   $0
 popf

 pushq  %rsp
	
 call   amd64_multiboot_main

 # Pop the boot stack pointer
 popq   %rcx

 # Start the system. We will never return.
 call   amd64_system_start

stop:
 sti
 hlt
 cli
 jmp    stop

