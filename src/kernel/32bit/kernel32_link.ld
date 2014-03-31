/* Copyright (c) 1997-2012, FenixOS Developers
   All Rights Reserved.

   This file is subject to the terms and conditions defined in
   file 'LICENSE', which is part of this source code package.
 */

/**
 * \file kernel32_link.ld This file contains the link script needed to link 
 * the 32-bit multiboot2 image.
 */

OUTPUT_ARCH ( "i386" )
OUTPUT_FORMAT("elf32-i386")
ENTRY(_start)

PHDRS
{
 text PT_LOAD FILEHDR PHDRS FLAGS(5);
 rodata PT_LOAD FLAGS(4);
 data PT_LOAD FLAGS(6);
}

SECTIONS
{
 . = SIZEOF_HEADERS;

 .text (0x001fe000 + SIZEOF_HEADERS) :
 {
  *.o (.text*)
  *.o (.rodata*)
  *.o (.eh_frame*)
  . = ALIGN(4096);
 } : text

 .rodata (LOADADDR(.text) + SIZEOF (.text)) :
 {
  main_kernel = .;
  */kernel/64bit/kernel.o (.data) 
 } : rodata

 .bss (LOADADDR(.rodata) + SIZEOF (.rodata)) :
 {
  . = ALIGN(4096);
  . = . + 4096;
  kernel_end = .;
 }

 /* Various debug sections. */
 .stab 0 : 
 { 
  * (.stab)
 }

 .stabstr 0 :
 { 
  * (.stabstr)
 }

 .stab.excl 0 :
 { 
  * (.stab.excl)
 }

 .stab.exclstr 0 :
 {
  * (.stab.exclstr)
 }

 .stab.index 0 :
 {
  * (.stab.index)
 }

 .stab.indexstr 0 :
 {
  * (.stab.indexstr)
 }

 .comment 0 :
 {
  * (.comment)
 }

 .debug 0 :
 {
  * (.debug)
 }

 .line 0 :
 {
  * (.line)
 }

 .debug_srcinfo 0 :
 {
  * (.debug_srcinfo)
 }

 .debug_sfnames 0 :
 {
  * (.debug_sfnames)
 }

 .debug_aranges 0 :
 {
  * (.debug_aranges)
 }

 .debug_pubnames 0 :
 {
  * (.debug_pubnames)
 }

 .debug_info 0 :
 {
  * (.debug_info .gnu.linkonce.wi.*)
 }

 .debug_abbrev 0 :
 {
  * (.debug_abbrev)
 }

 .debug_line 0 :
 {
  * (.debug_line)
 }

 .debug_frame 0 :
 {
  * (.debug_frame)
 }

 .debug_str 0 :
 {
  * (.debug_str)
 }

 .debug_loc 0 :
 {
  * (.debug_loc)
 }

 .debug_macinfo 0 :
 {
  * (.debug_macinfo)
 }

 .debug_weaknames 0 :
 {
  * (.debug_weaknames)
 }

 .debug_funcnames 0 :
 {
  * (.debug_funcnames)
 }

 .debug_typenames 0 :
 {
  * (.debug_typenames)
 }

 .debug_varnames 0 :
 {
  * (.debug_varnames)
 }

 .debug_pubtypes 0 :
 {
  * (.debug_pubtypes)
 }

 .debug_ranges 0 :
 {
  * (.debug_ranges)
 }

 .gnu.attributes 0 :
 { 
  KEEP (* (.gnu.attributes))
 }

 /DISCARD/ :
 {
  * (.note.GNU-stack)
  * (.gnu_debuglink)
  * (*)
 }
}
