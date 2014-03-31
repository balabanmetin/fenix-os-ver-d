/* Copyright (c) 1997-2012, FenixOS Developers
   All Rights Reserved.

   This file is subject to the terms and conditions defined in
   file 'LICENSE', which is part of this source code package.
*/

/* This is bad practice but to reduce the number of source code
   files we embedd here declarations which normally is located in
   header files. */

typedef char                   int8_t;
typedef unsigned char          uint8_t;
typedef short                  int16_t;
typedef unsigned short         uint16_t;
typedef long int               int32_t;
typedef unsigned long int      uint32_t;
typedef long long int          int64_t;
typedef unsigned long long int uint64_t;

#define MAX_NUMBER_OF_CPUS (16)

#define EI_NIDENT (16)    /*!< Number of entries in the identification array */

/*! Defines an ELF64 file header. */
struct Elf64_Ehdr
{
 uint8_t  e_ident[EI_NIDENT]; /*!< Array of bytes that shows that this
                                   is an ELF image and what type of
                                    ELF image it is. */
 uint16_t e_type;             /*!< The type of ELF executable image. */
 uint16_t e_machine;          /*!< Identifies the type of machine that
                                   the image can execute on. */
 uint32_t e_version;          /*!< The version of the ELF specification
                                   the image adheres to. */
 uint64_t e_entry;            /*!< Start address of the executable. */
 uint64_t e_phoff;            /*!< The offset into the image where the
                                   program header table is found. */
 uint64_t e_shoff;            /*!< The offset into the image where the
                                   section header table is found. */
 uint32_t e_flags;            /*!< Flags that are machine specific.
                                   These can be used to differentiate
                                   between similar machines. */
 uint16_t e_ehsize;           /*!< The size, in bytes, of the header. */
 uint16_t e_phentsize;        /*!< The size, in bytes, of each entry in
                                   the program header table. */
 uint16_t e_phnum;            /*!< The size, in entries, of the program
                                   header table. */
 uint16_t e_shentsize;        /*!< The size, in bytes, of each entry in
                                   the section header table. */
 uint16_t e_shnum;            /*!< The size, in entries, of the section
                                   header table. */
 uint16_t e_shstrndx;         /*!< The index, into the section table,
                                   of the section name string table.*/
};


/* Values used in p_type */
#define PT_LOAD (1)   /*!< The segment can be loaded into memory. */
/* Values used in p_flags */
#define PF_X        0x1        /*!< Segment can be executed.*/
#define PF_W        0x2        /*!< Segment can be written. */
#define PF_R        0x4        /*!< Segment can be read. */


/*! Defines an entry in the ELF64 program header table. Each entry
    corresponds to a segment. */
struct Elf64_Phdr
{
 uint32_t p_type;   /*!< Segments can have several types. p_type holds the
                         type. */
 uint32_t p_flags;  /*!< The attribute flags of the segment. */
 uint64_t p_offset; /*!< Offset into the image of the first byte of the
                         segment */
 uint64_t p_vaddr;  /*!< The (virtual) address to which the segment is
                               to be loaded. */
 uint64_t p_paddr;  /*!< Not used. */
 uint64_t p_filesz; /*!< The number of bytes the segment occupies in the
                         image. */
 uint64_t p_memsz;  /*!< The number of bytes the segment occupies in memory. */
 uint64_t p_align;  /*!< The alignment the segment should have in memory. This
                         field is currently being ignored. */
};

/*
 *  Some struts used when parsinf ACPI tables. For more information on
 *  ACPI see http://www.acpi.info .
 */

typedef struct
{
 char               signature[8];
 uint8_t            checksum;
 char               OEMID[6];
 uint8_t            revision;
 void*              rsdt_address;
 uint32_t           length;
 uint64_t           xsdt_address;
 uint8_t            extended_checksum;
 char               reserved[3];
} RSD;

typedef struct
{
 char               signature[4];
 uint32_t           length;
 uint8_t            revision;
 uint8_t            checksum;
 char               OEMID[6];
 char               OEM_table_id[8];
 uint32_t           OEM_revision;
 uint32_t           creator_id;
 uint32_t           creator_revision;
} DESCRIPTION_HEADER;

typedef struct
{
 DESCRIPTION_HEADER dheader;
 uint64_t           entry[0];
} XSDT;

typedef struct
{
 DESCRIPTION_HEADER  dheader;
 DESCRIPTION_HEADER* entry[0];
} RSDT;

typedef struct
{
 uint8_t             type;
 uint8_t             length;
} APIC_structure;

typedef struct
{
 DESCRIPTION_HEADER  dheader;
 uint32_t            local_APIC_base;
 uint32_t            flags;
 char                structures[0];
} MADT;

typedef struct
{
 APIC_structure      header;
 uint8_t             ACPI_processor_id;
 uint8_t             APIC_id;
 uint32_t            flags;
} processor_local_APIC_structure;

typedef struct
{
 APIC_structure      header;
 uint8_t             IO_APIC_id;
 uint8_t             reserved;
 uint32_t            IO_APIC_base;
 uint32_t            global_system_interrupt_base;
} IO_APIC_structure;

typedef struct
{
 APIC_structure      header;
 uint8_t             bus;
 uint8_t             source;
 uint32_t            global_system_interrupt;
 uint16_t            flags;
} ISO_structure;

struct CPU_private
{
 uint64_t                       activeContext;    // offset 0
 uint64_t                       scratchPad1;      // offset 8

 /*! The stack used in system call handlers. */
 uint64_t                       syscallStack;     // offset 16

 /*! The index of this processor. */
 uint64_t                       processorIndex;   // offset 24

 /*! The APIC id of the APIC connected to this processor. */
 uint64_t                       APICId;           // offset 32
};



/*! Defines a structure embedded into the 64-bit kernel. The structure holds
    pointers to global data structures. */
struct kernel_data_structures
{
 uint32_t magic;
 uint32_t version;
 uint64_t lowest_available_physical_memory;
 uint64_t top_of_available_physical_memory;
 uint64_t bsp_gdt;
 uint64_t bsp_gdt_size;
 uint64_t pic_interrupt_map;
 uint64_t cpu_private_data;
 uint64_t local_apic_base;
 uint64_t io_apic_base;
 uint64_t number_of_cpus;
};

/*! The main 64-bit kernel. We use this to extract information such as the
    the location of BSS segment. */
extern struct Elf64_Ehdr
main_kernel[1];

/*! Wrapper for the cpuid instruction. */
static inline void
cpuid(register const uint32_t function_number /*!< The number of the
                                                   requested function.
                                               */,
      register uint32_t *     EAX_value       /*!< The value returned
                                                   in the EAX register.
                                               */,
      register uint32_t *     EBX_value       /*!< The value returned
                                                   in the EBX register.
                                               */,
      register uint32_t *     ECX_value       /*!< The value returned
                                                   in the ECX register.
                                               */,
      register uint32_t *     EDX_value       /*!< The value returned
                                                   in the EDX register.
                                               */)
{
 __asm volatile("cpuid" :
                "=a" (*EAX_value),
                "=b" (*EBX_value),
                "=c" (*ECX_value),
                "=d" (*EDX_value) :
                "a" (function_number) : );
}

/*! Wrapper for a 8-bit out instruction. */
static inline void
out(register const int16_t port_number  /*!< The number of the port to
                                             write to. */,
    register const int8_t  output_value /*!< The value to be written to the
                                             port. */)
{
 __asm volatile("outb %%al,%%dx" : : "d" (port_number), "a" (output_value) : );
}

static inline void
print_string(register uint8_t *    screen_address,
             register const char * string)
{
 __asm volatile(" .extern amd64_print_string \n jmp amd64_print_string" : :
                "D" (screen_address), "S" (string) : "ecx");
}

static inline uint64_t *
convert_64_bit_pointer(register const uint64_t pointer)
{
 return (uint64_t *) ((uint32_t)(pointer - 0xffffffff80000000));
}

static inline void
clear_page(register uint64_t * page_address)
{
 register int counter;

 for(counter=0; counter<512; counter++)
  *page_address++=0;
}

static inline uint64_t *
allocate_page_table_memory(
 uint64_t ** const lowest_available_page_table_memory,
 const uint64_t    largest_kernel_address,
 const uint64_t    top_of_physical_memory)
{
 register uint64_t * const return_value = *lowest_available_page_table_memory;

 clear_page(*lowest_available_page_table_memory);

 *lowest_available_page_table_memory += 0x1000/sizeof(uint64_t);

 /* Check if we use up all of the memory below 0x1ff000. */
 if (0x1ff000 == ((uint32_t)(*lowest_available_page_table_memory)))
 {
  *lowest_available_page_table_memory = (uint64_t *)
   ((((uint32_t) (largest_kernel_address - 0xffffffff80000000)) + 4095) &
                 (-4096));
 }

 /* Check if we used up all of the memory all the way up to the stack.
    The adjustment of 4096 is so that we have a page left for stack. The
    system could crash if the stack is corrupted. */
 if (((uint32_t)(*lowest_available_page_table_memory)) >=
     (top_of_physical_memory + 4096))
 {
  print_string((uint8_t *) (0xb8000 + 160),
               "PANIC: Too little main memory to fit the initial page table.");
 }

 return return_value;
}


/*
 *  Some code which parses ACPI tables. For more information on
 *  ACPI see http://www.acpi.info .
 */

static int
strncmp(register const char* buffer, register const char* string,
        register unsigned int length)
{
 for(; length>0; length--, buffer++, string++)
 {
  if (*buffer != *string)
  {
   return 0;
  }
 }
 return 1;
}

static RSD*
search_for_RSD(register const char* area, register unsigned int length)
{
 for(;length>0; length-=16, area+=16)
 {
  /* Check the checksum. */
  register char checksum = 0;
  register int  i;

  for (i=0; i<20; i++)
  {
   checksum+=area[i];
  }

  /* Check if the checksum is correct. */
  if (0 != checksum)
   continue;

  if (0 == strncmp(area, "RSD PTR ", 8))
   continue;

  return (RSD*) area;
 }
 return (RSD*) 0;
}

static int
parse_description_header(
 const DESCRIPTION_HEADER* const table,
 register const struct kernel_data_structures* const kernel_data)
{
 register unsigned int i;
 register char         checksum = 0;
 register unsigned int curr_length;
 register int          return_value=1;

 if (0 == strncmp(table->signature, "APIC", 4))
  return 0;

 /* Check checksum. */
 for(i=0; i<table->length; i++)
 {
  checksum += ((char*) table)[i];
 }

 if (0 != checksum)
  return 0;

 /* Checksum is correct and we have found an MADT. Store the physical address
    of the APIC. */
 unsigned int const local_APIC_base = ((MADT*) table) -> local_APIC_base;

 /* Sanity check the address. */
 if (0 != (local_APIC_base & 0xfff))
  return 0;

 if (0xfec00000 !=  (local_APIC_base & 0xffc00000))
  return 0;

 if (0 == *convert_64_bit_pointer(kernel_data->local_apic_base))
 {
  *convert_64_bit_pointer(kernel_data->local_apic_base) = local_APIC_base;
 }
 else if (local_APIC_base !=
          *convert_64_bit_pointer(kernel_data->local_apic_base))
  /* We need to have all local APICs mapped on the same base address. */
  return 0;

 /* Now we need to parse all the entries in the table. */
 for(curr_length=44; curr_length < table->length;)
 {
  register const APIC_structure* const structure =
   ((APIC_structure*) &((MADT*) table) -> structures[curr_length-44]);

  curr_length += structure -> length;

  switch (structure -> type)
  {
   case 0: /* Processor local APIC */
   {
    register const processor_local_APIC_structure* const local_APIC_structure =
     (processor_local_APIC_structure*) structure;

    register struct CPU_private * const CPU_private_table =
     (struct CPU_private *)
      convert_64_bit_pointer(kernel_data->cpu_private_data);

    const uint64_t number_of_available_CPUs =
     *convert_64_bit_pointer(kernel_data->number_of_cpus);

    /* Sanity check the size of the structure. */
    if (8 != structure->length)
    {
     return_value=0;
     break;
    }

    /* Check if processor is disabled. */
    if (((local_APIC_structure->flags)&1) == 0)
     break;

    /* Check if the kernel cannot support more CPUs. */
    if (MAX_NUMBER_OF_CPUS <= number_of_available_CPUs)
     break;

    /* Extract the APIC information and initialize private data. */
    CPU_private_table[number_of_available_CPUs].syscallStack =
     0x200000 - 2*4096*number_of_available_CPUs;
    CPU_private_table[number_of_available_CPUs].processorIndex =
     number_of_available_CPUs;
    CPU_private_table[number_of_available_CPUs].APICId =
     local_APIC_structure->APIC_id;

    (*convert_64_bit_pointer(kernel_data->number_of_cpus))++;
    break;
   }

   case 1: /* I/O APIC */
   {
    register const IO_APIC_structure* const IO_APIC_structure_ptr =
     (IO_APIC_structure*) structure;

    /* Sanity check the size of the structure. */
    if (12 != structure->length)
    {
     return_value=0;
     break;
    }

    /* We can only handle one IO APIC. The IO APIC must handle interrupts
       starting from Global System Interrupt 0. */
    if ((0 != *convert_64_bit_pointer(kernel_data->io_apic_base)) ||
        (0 != IO_APIC_structure_ptr->global_system_interrupt_base))
    {
     return_value=0;
     break;
    }

    *convert_64_bit_pointer(kernel_data->io_apic_base) =
     IO_APIC_structure_ptr->IO_APIC_base;

    /* Sanity check the address. */
    if ((0 != (IO_APIC_structure_ptr->IO_APIC_base & 0xfff)) ||
        (0xfec00000 !=  (IO_APIC_structure_ptr->IO_APIC_base & 0xffc00000)))
    {
     return_value=0;
     break;
    }

    break;
   }

   case 2:
   {
    /* Interrupt Source Override. */
    register const ISO_structure* const iso_structure_ptr =
     (ISO_structure*) structure;

    register uint32_t * const pic_interrupt_map =
     (uint32_t *) convert_64_bit_pointer(kernel_data->pic_interrupt_map);
    /* Sanity check. */
    if ((10 != structure->length) ||
        (0 != iso_structure_ptr->bus) ||
        (15 < iso_structure_ptr->source))
    {
     return_value=0;
     break;
    }

    pic_interrupt_map[iso_structure_ptr->source]=
     iso_structure_ptr->global_system_interrupt;

    break;
   }

   case 3: /* NMI  */
   case 4: /* Local APIC NMI Structure. */
   {
    /* We just don't care about these. */
    break;
   }

   default:
    print_string((uint8_t *) (0xb8000 + 160),
                 "PANIC: Cannot parse ACPI information.");
  }
 }

 /* Did we actually find any useful information? */
 if ((0 == *convert_64_bit_pointer(kernel_data->io_apic_base)) ||
     (0 == *convert_64_bit_pointer(kernel_data->number_of_cpus)))
  return_value = 0;

 return return_value;
}


/*! Parse ACPI tables and extract the information needed to boot the system. */
static void
parse_acpi_tables(
 register const struct kernel_data_structures* const kernel_data)
{
 register char* EDBA = (char*)
                       (((unsigned long)(*((unsigned short*) 0x40e)))<<4);

 /* Search the first 1k of EDBA for the RSD. */
 register RSD* RSDP = search_for_RSD(EDBA, 1024);
 register int  done = 0;

 /* Check if we actually found the RSD. */
 if (0 == RSDP)
 {
  /* No we didn't. Search the BIOS. */
  RSDP = search_for_RSD((char*) 0xe0000, 0x20000);
 }

 if (0 == RSDP)
 {
  /* Couldn't find RSDP. We have to hard code entries. This could happen for
     some versions of Bochs or if ACPI is not available. */

   register uint32_t * const pic_interrupt_map =
   (uint32_t *) convert_64_bit_pointer(kernel_data->pic_interrupt_map);
  register struct CPU_private * const CPU_private_table =
   (struct CPU_private *)convert_64_bit_pointer(kernel_data->cpu_private_data);
  register unsigned int index;

  for (index=0; index<16; index++)
  {
   pic_interrupt_map[index]=index;
  }

  /* The PIT timer is remapped to IO-APIC pin 2. */
  pic_interrupt_map[0]=2;

  *convert_64_bit_pointer(kernel_data->local_apic_base) = 0xfee00000ULL;
  *convert_64_bit_pointer(kernel_data->io_apic_base) = 0xfec00000ULL;
  *convert_64_bit_pointer(kernel_data->number_of_cpus) = 1;

  for (index=0; index<4; index++)
  {
   CPU_private_table[index].syscallStack = 0x200000 - 2*4096*index;
   CPU_private_table[index].processorIndex = index;
   CPU_private_table[index].APICId = index;
  }
  return;
 }

 /* Initialize the data structures. */
 {
  register uint32_t * const pic_interrupt_map =
   (uint32_t *) convert_64_bit_pointer(kernel_data->pic_interrupt_map);
  register unsigned int index;
  for (index=0; index<16; index++)
  {
   pic_interrupt_map[index]=index;
  }
 }

 /* We have found the RSDP. Now we need to figure out if we should use the
    RSDT or the XSDT. We always use XSDT if we can find it and it is
    located within the first 4GB. */
 {
  /* Check the extended checksum. */
  register char checksum = 0;
  register int  i;

  for (i=0; i<36; i++)
  {
   checksum+=((char*) RSDP)[i];
  }

  /* Check if the checksum is correct and the XSDT is within the . */
  if ((0 == checksum) && (RSDP->xsdt_address < (0x100000000ULL-sizeof(XSDT))))
  {
   register const XSDT* const xsdt = (XSDT*) ((long)(RSDP->xsdt_address&
                                                     0xffffffff));

   /* Check if the XSDT is valid. */
   if ((0 != strncmp(xsdt->dheader.signature, "XSDT", 4)) &&
       ((xsdt->dheader.length&3) == 0))
   {
    /* Check checksum. */
    register unsigned int i;
    register char         checksum = 0;

    for(i=0; i<xsdt->dheader.length; i++)
    {
     checksum += ((char*) xsdt)[i];
    }

    if (0 == checksum)
    {
     /* Calculate the number of entries. */
     register unsigned int entries = (xsdt->dheader.length-36) >> 3;
     /* Go through the DESCRIPTION_HEADERS looking for ACPI tables. */
     for(i=0; i<entries; i++)
     {
      uint32_t entry;
      if (xsdt->entry[i] >0x100000000ULL)
       continue;

      entry = xsdt->entry[i] & 0xffffffff;

      done |= parse_description_header((DESCRIPTION_HEADER*) (entry),
                                       kernel_data);
     }

     /* Return if we could parse the table. */
     if (0 != done)
      return;
    }
   }
  }
 }

 /* Couldn't use the XSDT. Fall back to the RSDT. The RSDT only contains 32-bit
    physical addresses so we do not have to check if they are accessible. */
 {
  register const RSDT* const rsdt = (RSDT*) (RSDP->rsdt_address);

  /* Check if the RSDT is valid. */
  if ((0 != strncmp(rsdt->dheader.signature, "RSDT", 4)) &&
      ((rsdt->dheader.length&3) == 0))
  {
   /* Check checksum. */
   register unsigned int i;
   register char         checksum = 0;

   for(i=0; i<rsdt->dheader.length; i++)
   {
    checksum += ((char*) rsdt)[i];
   }

   if (0 == checksum)
   {
    /* Calculate the number of entries. */
    register unsigned int entries = (rsdt->dheader.length-36) >> 2;

    /* Go through the DESCRIPTION_HEADERS looking for ACPI tables. */
    for(i=0; i<entries; i++)
    {
     done |= parse_description_header(rsdt->entry[i], kernel_data);
    }

    /* Return if we could parse the table. */
    if (0 != done)
     return;
   }
  }
 }

 print_string((uint8_t *) (0xb8000 + 160),
              "PANIC: Cannot parse ACPI information.");
}


static void
map_address_range(
 register uint64_t * const   page_table_root,
 register uint64_t ** const  lowest_available_page_table_memory,
 register const uint64_t     largest_kernel_address,
 register const uint64_t     top_of_physical_memory,
 register uint64_t           virtual_address,
 register uint64_t           length,
 register uint64_t           physical_address,
 register const uint64_t     protection_bits,
 register const unsigned int one_gig_pages);

static uint32_t
map_page(register uint64_t * const   table_pointer,
         register uint64_t * const   page_table_root,
         register uint64_t ** const  lowest_available_page_table_memory,
         register const uint64_t     largest_kernel_address,
         register const uint64_t     top_of_physical_memory,
         register const uint64_t     virtual_address,
         register const uint64_t     length,
         register const uint64_t     physical_address,
         register const uint64_t     protection_bits,
         register const unsigned int level,
         register const unsigned int one_gig_pages)
{
 while(1)
 {
  register uint64_t table_entry =
   *(table_pointer + ((virtual_address>>(12+9*level))&0x1ff));

  if (table_entry & 1)
  {
   register unsigned int match = 0;
   register uint64_t     page_size = 4096;
   register uint64_t     large_page_protection_bits = 0;

   /* Figure out if the tree is already terminated with a page which matches
      the protection that we need. */
   if (0 == level)
   {
    /* Simple case, 4K page. */
    match = protection_bits == (table_entry & 0x800000000000019fULL);
   }
   else if (0x80 == (0x80 & table_entry))
   {
    /* Larger page, we need to recode the protection bits. */
    large_page_protection_bits =
     ((protection_bits & 0x80) << 5) | (protection_bits & (~0x80));

    /* One gig page? */
    if (level>1)
     page_size = 1*1024*1024*1024;
    else
     page_size = 2*1024*1024;

    match = large_page_protection_bits ==
            (table_entry & 0x800000000000111fULL);
   }
   else
   {
    /* Descend into page table. */
    return map_page((uint64_t *)((uint32_t)(table_entry &
                                            0x7ffffffffffff000ULL)),
                    page_table_root,
                    lowest_available_page_table_memory,
                    largest_kernel_address,
                    top_of_physical_memory,
                    virtual_address,
                    length,
                    physical_address,
                    protection_bits,
                    level-1,
                    one_gig_pages);

   }

   if (match)
   {
    /* We are done and happy. */
    return page_size;
   }
   else
   {
    if ((0 == level) || (length >= page_size))
    {
     /* Conflicting memory information. We let the final assignment rule. */
     if (0 == level)
     {
      *(table_pointer + ((virtual_address>>(12+9*level))&0x1ff)) =
       physical_address | protection_bits;
     }
     else
     {
      *(table_pointer + ((virtual_address>>(12+9*level))&0x1ff)) =
       physical_address | large_page_protection_bits;
     }
     return page_size;
    }

    /* Generate a new level. */
    *(table_pointer + ((virtual_address>>(12+9*level))&0x1ff)) =
     (uint32_t)(allocate_page_table_memory(lowest_available_page_table_memory,
                                           largest_kernel_address,
                                           top_of_physical_memory)) | 7ULL;

    /* Remap the page. This will break it up. */
    map_address_range(page_table_root,
                      lowest_available_page_table_memory,
                      largest_kernel_address,
                      top_of_physical_memory,
                      virtual_address & (-page_size),
                      page_size,
                      physical_address & (-page_size),
                      protection_bits,
                      one_gig_pages);

    /* Retry mapping the memory. */
    continue;
   }
  }
  else
  {
   /* Simple case: bottom of the tree. Always 4k pages. */
   if (0 == level)
   {
    *(table_pointer + ((virtual_address>>(12+9*level))&0x1ff)) =
     physical_address | protection_bits;
    return 4096;
   }

   /* Figure out if we can terminate the tree with a large page. */
   if (((1 == level) && (length >= 2*1024*1024)) ||
       (one_gig_pages && (2 == level) && (length >= 1*1024*1024*1024)))
   {
    *(table_pointer + ((virtual_address>>(12+9*level))&0x1ff)) =
     physical_address | protection_bits | 0x80ULL;

    return (1 == level) ? 2*1024*1024 : 1*1024*1024*1024;
   }
   else
   {
    /* No, we cannot terminate with a large page. Need to create a new level
       in the tree. */
    register uint64_t * page_table =
     allocate_page_table_memory(lowest_available_page_table_memory,
                                largest_kernel_address,
                                top_of_physical_memory);

    *(table_pointer + ((virtual_address>>(12+9*level))&0x1ff)) =
     (uint32_t)(page_table) | 7ULL;

    map_page(page_table,
             page_table_root,
             lowest_available_page_table_memory,
             largest_kernel_address,
             top_of_physical_memory,
             virtual_address,
             length,
             physical_address,
             protection_bits,
             level-1,
             one_gig_pages);
   }
  }
 }
}


static void
map_address_range(
 register uint64_t * const   page_table_root,
 register uint64_t ** const  lowest_available_page_table_memory,
 register const uint64_t     largest_kernel_address,
 register const uint64_t     top_of_physical_memory,
 register uint64_t           virtual_address,
 register uint64_t           length,
 register uint64_t           physical_address,
 register const uint64_t     protection_bits,
 register const unsigned int one_gig_pages)
{
 while(length > 0)
 {
  const register uint32_t mapped_length =
   map_page(page_table_root,
            page_table_root,
            lowest_available_page_table_memory,
            largest_kernel_address,
            top_of_physical_memory,
            virtual_address,
            length,
            physical_address,
            protection_bits,
            3,
            one_gig_pages);

  if (mapped_length > length)
   return;

  virtual_address += mapped_length;
  physical_address += mapped_length;
  length -= mapped_length;
 }
}

extern uint32_t __attribute__((fastcall))
parse_multiboot_information_and_setup_long_mode(
 register const uint32_t * const multiboot_information,
 register const uint32_t         stack_pointer
  /*!< The initial stack pointer which is pointing to the top of memory. */)
{
 /* Holds the highest address used by the kernel itself. */
 register uint64_t largest_kernel_address = 0xffffffff80000000;

 /* Holds the top of accessible memory. */
 register uint64_t top_of_physical_memory = stack_pointer;

 /* Points to the 64-bit kernels struct kernel_data_structure. */
 register const struct kernel_data_structures * kernel_data_structures;

 /* True iff 1GB pages are supported. */
 register int      one_gig_pages = 0;

 /* Set up the timer hardware to generate interrupts roughly 200 times a
    second. */
 out(0x43, 0x36);
 out(0x40, 78);
 out(0x40, 23);

 /* Turn off all PIC interrupts. We will use the IO-APIC. */
 out(0x20, (int8_t) 0x11);
 out(0xA0, (int8_t) 0x11);

 out(0x21, (int8_t) 0x20);
 out(0xA1, (int8_t) 0x28);

 out(0x21, (int8_t) (1<<2));
 out(0xA1, (int8_t) 2);

 out(0x21, (int8_t) 1);
 out(0xA1, (int8_t) 1);

 out(0x21, (int8_t) 0xff);
 out(0xA1, (int8_t) 0xff);

 /* Check CPUID information and verify that this processor actually can run the
    kernel. */
 {
  uint32_t EAX, EBX, ECX, EDX;

  /* Check if cpuid supports more the lowest standard function. */
  cpuid(0, &EAX, &EBX, &ECX, &EDX);
  if (EAX<1)
   print_string((uint8_t *) 0xb8000 + 160,
                "PANIC: CPUID information can not be retrieved.");

  /* Check if the CPU supports the features we need. */
  cpuid(1, &EAX, &EBX, &ECX, &EDX);
  if (0x0380a979 != (EDX & 0x0380a979))
   print_string((uint8_t *) 0xb8000 + 160,
                "PANIC: The CPU does not support the kernel.");

  /* Check if cpuid supports the extended functions we need to inquiry. */
  cpuid(0x80000000, &EAX, &EBX, &ECX, &EDX);
  if (EAX<0x80000001)
   print_string((uint8_t *) 0xb8000 + 160,
                "PANIC: CPUID information can not be retrieved.");

  cpuid(0x80000001, &EAX, &EBX, &ECX, &EDX);
  if (1 != ((EDX>>20)&1))
   print_string((uint8_t *) 0xb8000 + 160,
                "PANIC: NX bit is not supported.");
  if (1 != ((EDX>>29)&1))
   print_string((uint8_t *) 0xb8000 + 160,
                "PANIC: Long mode is not supported.");
  /* Everything looks ok! */

  /* Check for 1 gigabyte pages. */
  if (1 == ((EDX>>26)&1))
   one_gig_pages = 1;

 }

 /* Extract information from the ELF header of the 64-bit kernel. */
 {
  register uint16_t program_header_index;

  /* Parse the program header. */
  for(program_header_index = 0;
      program_header_index < (main_kernel->e_phnum);
      program_header_index++)
  {
   const struct Elf64_Phdr * const program_header_ptr =
    (struct Elf64_Phdr *)(((uint8_t *) main_kernel) + (main_kernel->e_phoff) +
                          program_header_index * (main_kernel->e_phentsize));

   if (PT_LOAD == (PT_LOAD & program_header_ptr->p_type))
   {
    /* Check if this segment will extend the size of the kernel. */
    if (largest_kernel_address <
        (program_header_ptr->p_vaddr + program_header_ptr->p_memsz))
    {
     largest_kernel_address =
      program_header_ptr->p_vaddr + program_header_ptr->p_memsz;

    /* Check if the segment fits in memory. Please note that GRUB will not
       do this as the 64-bit kernel is embedded into the 32-bit kernel. */
     if ((largest_kernel_address - 0xffffffff80000000) > stack_pointer)
      print_string((uint8_t *) 0xb8000 + 160,
                   "PANIC: Main memory is too small.");
    }

    if (program_header_ptr->p_filesz < program_header_ptr->p_memsz)
    {
     /* A segment which should be loaded into memory and has a larger memory
        footprint than file footprint. This must be the BSS. */

     /* Clear the BSS of the 64-bit kernel. */
     {
      /* First clear bytes up to an address which is 32-bit word aligned. */
      register uint8_t * bss_ptr =
       (uint8_t *) ((uint32_t)
       ((program_header_ptr->p_vaddr - 0xffffffff80000000) +
        (program_header_ptr->p_filesz)));
      for(; 0 != (((uint32_t) bss_ptr) & 3);)
       *bss_ptr++=0;
     }

     {
      /* Then the rest of the BSS. */
      register uint32_t * bss_ptr =
       (uint32_t *) ((uint32_t)
       ((((program_header_ptr->p_vaddr - 0xffffffff80000000) +
         (program_header_ptr->p_filesz) + 3) & (-4))));
      register uint64_t current_offset =
       (program_header_ptr->p_filesz + 3) & (-4);

      /* This will clear extra bytes but we do not care about that. */
      for(;
          current_offset < ((program_header_ptr->p_memsz + 4095) & (-4096));
          current_offset+=4)
       *bss_ptr++=0;
     }
    }
   }
  }
 }

 largest_kernel_address -= 0xffffffff80000000ULL;

 /* Find the struct kernel_data_structures structure at the start of the
    64-bit kernel. It holds pointers to important data structures. */
 kernel_data_structures = (struct kernel_data_structures *)
  (((uint8_t *)(convert_64_bit_pointer(main_kernel->e_entry))) -
   sizeof(struct kernel_data_structures));

 /* Check that the pointer is valid. */
 if (((main_kernel->e_entry & 7) != 0) ||
     (kernel_data_structures->magic != 0x786e6546) ||
     (kernel_data_structures->version != 0x10000))
  print_string((uint8_t *) 0xb8000 + 160,
               "PANIC: 64-bit kernel is corrupt.");

 /* The initial stack pointer is pointing to the top of the memory so we
    can use it as top of available memory. */
 *(convert_64_bit_pointer(kernel_data_structures->
                          top_of_available_physical_memory)) = stack_pointer;
 /* We have extracted all information we need to extract from GRUB so we can
    start using all memory. */
 {
  /* Build the inital page table. */

  /* We use 0x100000 to 0x1ff000 first and then the memory located after the
     kernel. We also use 2 megabyte pages or, if they are available, 1
     gigabyte pages. */

  /* Pointers to the different levels of the page table tree. */
  register uint64_t * const PML4T_ptr = (uint64_t *) 0x100000;
  uint64_t *                lowest_available_page_table_memory =
                             (uint64_t *) 0x101000;
  register uint16_t         program_header_index;

  /* The root of the page table. */
  clear_page(PML4T_ptr);

  /* We start mapping all of the phyical memory allocating memory for the
     tables as we go. */

  map_address_range(PML4T_ptr,
                    &lowest_available_page_table_memory,
                    largest_kernel_address,
                    top_of_physical_memory,
                    0,
                    top_of_physical_memory,
                    0,
                    7ULL,
                    one_gig_pages);

  /* Traverse the memory map. We will pass information on to the 64-bit kernel
     while we traverse the map. */

  /* We add a mapping for the kernel. We does that by going through the ELF
     program header and creating the tables as we go. */
  /* Extract information from the ELF header of the 64-bit kernel. */

  /* Parse the program header. */
  for(program_header_index = 0;
      program_header_index < (main_kernel->e_phnum);
      program_header_index++)
  {
   const struct Elf64_Phdr * const program_header_ptr =
   (struct Elf64_Phdr *)(((uint8_t *) main_kernel) + (main_kernel->e_phoff) +
                         program_header_index * (main_kernel->e_phentsize));

   if (PT_LOAD == (PT_LOAD & program_header_ptr->p_type))
   {
    register uint64_t protection_bits = 1;

    if (PF_W == (PF_W & program_header_ptr->p_flags))
     protection_bits |= 2;

    if (PF_X != (PF_X & program_header_ptr->p_flags))
     protection_bits |= 0x8000000000000000ULL;

    /* All PT_LOAD segments have to be mapped. */
    map_address_range(PML4T_ptr,
                      &lowest_available_page_table_memory,
                      largest_kernel_address,
                      top_of_physical_memory,
                      program_header_ptr->p_vaddr,
                      program_header_ptr->p_memsz,
                      program_header_ptr->p_vaddr - 0xffffffff80000000ULL,
                      protection_bits,
                      one_gig_pages);
   }
  }

  /* Extract ACPI information. */
  parse_acpi_tables(kernel_data_structures);

  /* Map the APIC address ranges. */
  map_address_range(PML4T_ptr,
                    &lowest_available_page_table_memory,
                    largest_kernel_address,
                    top_of_physical_memory,
                    0xfec00000ULL,
                    0x400000ULL,
                    0xfec00000ULL,
                    0x8000000000000003ULL,
                    one_gig_pages);

  /* Check if we need to adjust the end of the 64-bit kernel memory. */
  if (lowest_available_page_table_memory > ((uint64_t *)0x200000))
  {
   largest_kernel_address =
    (uint64_t)((uint32_t) lowest_available_page_table_memory);
  }
 }

 /* We have now used all of the memory we need so far. We can inform the 64-bit
    kernel about the low limit of the available memory.
  */
 *(convert_64_bit_pointer(kernel_data_structures->
                          lowest_available_physical_memory)) =
                                            largest_kernel_address;

 /* First enable 64-bit page table entries by setting the PAE bit. We also
    enable the 128-bit floating point instructions. */
 __asm volatile("movl    %%cr4,%%eax \n \
                 bts     $5,%%eax # Enable 64-bit page table entries \n \
                 bts     $9,%%eax # Enable 128-bit floating point \n \
                                  # instructions \n \
                 movl    %%eax,%%cr4" : : : "eax");

 {
  /* Build the BSP GDT. */
  uint64_t * gdt = convert_64_bit_pointer(kernel_data_structures->bsp_gdt);

  /* Long mode user mode descriptor for a 64-bit data segment */
  gdt[1] = 0x008ff2000000ffffULL;

  /* Long mode user mode descriptor for a 64-bit code segment */
  gdt[2] = 0x00affb000000ffffULL;

  /* Long mode supervisor mode descriptor for a 64-bit data segment */
  gdt[3] = 0x00af9b000000ffffULL;

  /* Long mode supervisor mode descriptor for a 64-bit code segment */
  gdt[4] = 0x008f92000000ffffULL;

  /* 32-bit protected mode code segment. */
  gdt[5+16*2] = 0x00cf9a010000ffffULL;

  /* 32-bit protected mode dat segment. */
  gdt[6+16*2] = 0x00cf92000000ffffULL;

  /* Set up the GDT. */
  {
   uint32_t gdt_limit = (kernel_data_structures->bsp_gdt_size) - 1;
   __asm volatile("sub $8,%%esp\n \
                   mov %%eax,4(%%esp)\n \
                   mov %%bx,2(%%esp)\n \
                   lgdt 2(%%esp)\n \
                   add $8,%%esp" :
                  : "a"(gdt), "b"(gdt_limit):);
  }

 /* Set the page table root pointer. */
 __asm volatile("mov %%eax,%%cr3" : : "a"(0x100000) :);

 /* Enable long mode. This is done by setting a bit in the EFER register
    The EFER register is accessed indirectly. We also enable syscall/sysret
    and the use of the NX bit. */
 __asm volatile("rdmsr   # Read EFER \n \
                 orl     $0x901,%%eax \n \
                 wrmsr   # Write EFER." : : "c"(0xc0000080) : "eax", "edx");


  /* Enable paging, write protection etc. This will also activate 64-bit mode.
   */
  __asm volatile("mov %%eax,%%cr0" : : "a" (0x80010033) :);

 }

 /* Return the ELF image entry point. */
 return (uint32_t)(main_kernel->e_entry - 0xffffffff80000000);
}
