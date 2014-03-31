/* Copyright (c) 1997-2012, FenixOS Developers
   All Rights Reserved.

   This file is subject to the terms and conditions defined in
   file 'LICENSE', which is part of this source code package.
 */

/**
 * \file kernel64_link.ld This file contains the link script used to link
 * the 64-bit kernel.
 */

OUTPUT_ARCH ( "i386:x86-64" )
OUTPUT_FORMAT("elf64-x86-64")
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

  .text (0xffffffff80200000 + SIZEOF_HEADERS + SIZEOF(.startap)) :
  {
   *.o (.text*)
  . = ALIGN(4096);
  } : text

  .rodata (LOADADDR(.text) + SIZEOF (.text)) :
  {
   *.o (.rodata*)
   *.o (.eh_f*)
   . = ALIGN(8);
   constructor_start_array = .;
   * (.ctors)
   QUAD(0);
   ELF_images = ABSOLUTE(.);
   QUAD(_program_0_executable_start);
   QUAD(_program_1_executable_start);
   QUAD(_program_2_executable_start);
   QUAD(0);
   . = ALIGN(4096);
   _program_0_executable_start = .;
   *program_0/executable.o (.data)
   . = ALIGN(4096);
   _program_1_executable_start = .;
   *program_1/executable.o (.data)
   . = ALIGN(4096);
   _program_2_executable_start = .;
   *program_2/executable.o (.data)
   . = ALIGN(4096);
  } : rodata

  .data (LOADADDR(.rodata) + SIZEOF (.rodata)) :
  {
   *.o (.data*)
  } : data

  .bss (LOADADDR(.data) + SIZEOF (.data)) :
  {
   *.o (.bss*)
  } : data

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

