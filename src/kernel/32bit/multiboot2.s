# Copyright (c) 1997-2012, FenixOS Developers
# All Rights Reserved.
#
# This file is subject to the terms and conditions defined in
# file 'LICENSE', which is part of this source code package.

/**
 * \file  multiboot2.s This file contains the multiboot 2 table and code that
 *  allows the bootstrap processor to start executing in long mode. While
 *  initializing, various sanity checks are performed. Most of the
 *  initialization is done in C code found in the setup_long_mode.c file.
 */

# Macros

# Print text to the VGA screen.
.macro print screen,string,temp
loop\@:
 mov     (\string),\temp
 test    \temp,\temp
 jz      end\@
 mov     \temp,(\screen)
 inc     \string
 add     $2,\screen
 jmp     loop\@
end\@:
 .endm

	
 .text
 .global _start
 .global amd64_print_string
 .align  8

multiboot2_header:	
# Multiboot 2 header
 .int    0xe85250d6                         # Magic
 .int    0                                  # Architecture
 .int    multiboot2_header_end-multiboot2_header # Length
 .int    -(0xe85250d6+0+multiboot2_header_end-multiboot2_header) # Checksum
 .align  8

 .short  1                                  # Information request
 .short  0
 .int    20
 .int    4                                  # BASIC_MEMINFO
 .int    6                                  # MMAP
 .int    15                                 # ACPI_NEW
 .int    0                                  # END
	                                    # The spec is not clear about if
	                                    # the END tag is needed but grub
	                                    # complains if it is not there.
 .align  8

# The last tag
 .short  0                                  # End tag
 .short  0
 .int    8
multiboot2_header_end:

 .align  4
# Print text to the VGA screen and then halt.
amd64_print_string:
 print   %edi,%esi,%cl	
# We will end up here if something bad happens.
halt_the_machine:
 hlt                        # halt the machine
 jmp     halt_the_machine
	
/**
 * Entry point used by the multiboot 2 loader.
 */
 .align  4
_start:
 /* Disable interrupts. This should already been done by GRUB but lets be
    thorough. */
 cli	
 # Try to clear the screen memory
 mov     $0xb8000, %edi
 mov     $80*25,%edx
 mov     $0x0f20,%cx
scr_clear_loop:
 mov     %cx,(%edi)
 add     $2,%edi
 dec     %edx
 jnz     scr_clear_loop

 # Print greeting.	
 mov     $0xb8000,%edi
 mov     $Fenix_text,%esi
 print   %edi,%esi,%cl

 # Set up an error message to show if something goes wrong.	
 mov     $0xb8000+160,%edi
 mov     $Multiboot_text,%esi
	
 # Check that the boot loader put the expected magic number in eax.
 cmpl    $0x36d76289,%eax
 jne     amd64_print_string   # Ouch, no multiboot capable loader!

 # Figure out the highest usable address under the 4GB limit and put the
 # stack there. We do that by parsing the multiboot information.
 mov     $0xb8000+160,%edi
 mov     $Multiboot_parse_error_text,%esi

 # Calculate the end of the structure.
 mov     (%ebx),%eax
 add     %ebx,%eax
	
 mov     %ebx,%ecx
 add     $8,%ecx

 # Loop through all tags trying to find BASIC_MEMINFO.
check_next_tag:
 # Check for the end of the structure.
 cmp     %eax,%ecx
 jns     amd64_print_string

 # Check for end tag.
 cmpl    $0,(%ecx)	
 jz      amd64_print_string

 # Check for BASIC_MEMINFO.
 cmpl    $4,(%ecx)
 jz      found_basic_meminfo

 # Skip to the next tag.
 add     4(%ecx),%ecx
 add     $7,%ecx
 and     $-8,%ecx
 jmp     check_next_tag
	
found_basic_meminfo:
 # Perform various sanity checks on the extracted memory information.
 mov     12(%ecx),%esp
 # Sanity check the memory information.
 mov     $Too_little_memory,%esi
 cmp     $0,%esp
 jle     amd64_print_string

 # Check if the reported memory is larger than 4G-1M.
 cmp     $4*1024*1024-2*1024,%esp
 jle     no_saturation
 mov     $4*1024*1024-2*1024,%esp

no_saturation:
 shl     $10,%esp         # multiply by 1024
 add     $0x100000,%esp   # add the base
 and     $0xffffe000,%esp # Make sure esp is pointing to a page frame

 # Clear a page worth of memory so that the stack has a defined state. This is
 # a little bit of overkill.
 mov     $1024,%esi
 xor     %ecx,%ecx
clear_stack_loop:
 push    %ecx
 dec     %esi
 jnz     clear_stack_loop

 # Check if we have too little memory to start the kernel.	
 mov     $Too_little_memory,%esi
 cmp     $kernel_end,%esp
 jle     amd64_print_string

 # Adjust the stack pointer back to the top of memory.
 add     $4096,%esp	
	
 # Verify that we can use CPUID.

 # This is a little tricky. We can test if the CPUID instruction is available
 # by testing if bit 21 in EFLAGS can be changed.
 mov     $CPUID_text,%esi
 pushf
 pop     %ecx
 btr     $21,%ecx
 push    %ecx
	
 popf
 pushf
 pop     %ecx
 btc     $21,%ecx
 jb      amd64_print_string
	
 push    %ecx
 popf
 pushf
 pop     %ecx
 bt      $21,%ecx
 jae     amd64_print_string

 # GCC's fastcall use ecx and edx for parameters so we need to copy ebx to ecx.
 mov     %ebx,%ecx
 mov     %esp,%edx
 call    parse_multiboot_information_and_setup_long_mode

	# We can now do a long jump into the 64-bit code.
 ljmp    $24,$start_of_64bit_code

Fenix_text:
 .asciz "Course AMD64 kernel V0.5.0 is booting. Based on FenixOS technology..."
Multiboot_text:
 .asciz "PANIC: Fenix was not loaded by a multiboot 2 compatible bootloader."
CPUID_text:
 .asciz "PANIC: Processor does not seem to support CPUID."
Multiboot_parse_error_text:
 .asciz "PANIC: Fenix can not parse multiboot information."
Too_little_memory:
 .asciz "PANIC: Too little memory detected!"
