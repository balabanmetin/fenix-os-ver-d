#include "globals.h"

void
copy_ELF(const struct Elf64_Ehdr* const elf_image, uint64_t * entry_point, uint64_t * memory_block)
{
 /* Get the address of the program header table. */
 int                            program_header_index;
 const struct Elf64_Phdr* const program_header = ((const struct Elf64_Phdr*)
                                                  (((char*) (elf_image)) +
                                                  elf_image->e_phoff));
 unsigned long                  used_memory = 0;
 uint64_t                       memory_footprint_size = 0;
 long                           address_to_memory_block;

 /* Figure out how much memory should be allocated. */
 for (program_header_index = 0;
      program_header_index < elf_image->e_phnum;
      program_header_index++)
 {
  /* Check that all PT_LOAD segments are contiguous starting from
     address 0. Also, calculate the memory footprint of the image. */
  if (program_header[program_header_index].p_type == PT_LOAD)
  {
   if (program_header[program_header_index].p_vaddr !=
       memory_footprint_size)
   {
    while (1)
    {
     kprints("Kernel panic! Executable image has illegal memory layout.\n");
    }
   }

   memory_footprint_size += program_header[program_header_index].p_memsz;
  }
 }

 /* Allocate memory. */
 address_to_memory_block = kalloc(memory_footprint_size);
 /* Save the address to the parameter */
 *memory_block=address_to_memory_block;

 /* First check that we have enough memory. */
 if (0 >= address_to_memory_block)
 {
  /* No, we don't. */
  return;
 }

 /* Scan through the program header table and copy all PT_LOAD segments to
    memory. Perform checks at the same time.*/

 for (program_header_index = 0;
      program_header_index < elf_image->e_phnum;
      program_header_index++)
 {
  if (PT_LOAD == program_header[program_header_index].p_type)
  {
   /* Calculate destination adress. */
   unsigned long* dst = (unsigned long *) (address_to_memory_block +
                                           used_memory);

   /* Check for odd things. */
   if (
       /* Check if the segment is contigous */
       (used_memory != program_header[program_header_index].p_vaddr) ||
       /* Check if the segmen fits in memory. */
       (used_memory + program_header[program_header_index].p_memsz >
        memory_footprint_size) ||
       /* Check if the segment has an odd size. We require the segement
          size to be an even multiple of 8. */
       (0 != (program_header[program_header_index].p_filesz&7)))
   {
    /* Something went wrong. Panic. */
    while(1)
    {
     kprints(
"Kernel panic: Trying to create a process out of a corrupt executable image!");
    }
   }

   /* First copy p_filesz from the image to memory. */
   {
    /* Calculate the source address. */
    unsigned long* src = (unsigned long *) (((char*) elf_image)+
     program_header[program_header_index].p_offset);
    unsigned long count = program_header[program_header_index].p_filesz/8;

    for(; count>0; count--)
    {
     *dst++=*src++;
    }
   }


   /* Then write p_memsz-p_filesz bytes of zeros. This to pad the segment. */
   {
    unsigned long count = (program_header[program_header_index].p_memsz-
                           program_header[program_header_index].p_filesz);

    char * dst2 = (char *) dst;

    for(; count>0; count--)
    {
     *dst2++=0;
    }
   }

   /* Finally update the amount of used memory. */
   used_memory += program_header[program_header_index].p_memsz;
  }
 }

 /* Find out the address to the first instruction to be executed. */
 *entry_point =  address_to_memory_block + elf_image->e_entry;
}
