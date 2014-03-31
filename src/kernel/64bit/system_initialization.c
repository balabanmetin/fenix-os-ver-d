/* Copyright (c) 1997-2012, FenixOS Developers
   All Rights Reserved.

   This file is subject to the terms and conditions defined in
   file 'LICENSE', which is part of this source code package.
*/

/*! \file system_initialization.c This file holds code that is used when the
    system is initialized.
 */

#include "globals.h"
#include "instruction_wrappers.h"

uint64_t
amd64_BSP_GDT[7+2*16]  __attribute__((aligned (16)));

extern char
amd64_interrupt_entries[1];

uint64_t
amd64_BSP_GDT[7+2*16]  __attribute__((aligned (16)));

uint64_t
amd64_lowest_available_physical_memory;

uint64_t
amd64_top_of_available_physical_memory;

static uint64_t
amd64_IDT[2*256] __attribute__((aligned (16)));

static volatile unsigned int 
number_of_initialized_CPUs;

volatile uint64_t
amd64_AP_processor_index;

volatile uint64_t
amd64_AP_processor_index;

volatile uint32_t *
amd64_io_apic_address;

volatile uint32_t *
amd64_io_apic_address;

volatile uint32_t *
amd64_local_APIC_base_address;

struct AMD64KernelGSData
amd64_CPU_private_table[16];

/*!< This pointers are used during calculation of addresses in kmalloc and kfree
 * functions. */

blockPtr base=0;//used in free and alloc
blockPtr last;

uint64_t
amd64_number_of_available_CPUs;

uint64_t
amd64_number_of_available_CPUs;

uint32_t
amd64_pic_interrupt_map[16];
/*!< This array maps 8259 interrupts to ACPI Global System Interrupts. */

uint32_t
amd64_pic_interrupt_map[16];

extern char
amd64_start_application_processor[1];

extern char
amd64_start_application_processor_end[1];

/*! Initialize the local APIC for the current CPU. */
void
initialize_APIC(void)
{
 /* Make sure the APIC is enabled. */
 *(amd64_local_APIC_base_address + 0xf0/sizeof(unsigned int)) |= 0x100;

 /* Set task priority. */
 *(amd64_local_APIC_base_address + 0x80/sizeof(unsigned int)) = 0;

 /* Mask all local sources. */
 *(amd64_local_APIC_base_address + 0x320/sizeof(unsigned int)) |= 0x10000;
 *(amd64_local_APIC_base_address + 0x330/sizeof(unsigned int)) |= 0x10000;
 *(amd64_local_APIC_base_address + 0x340/sizeof(unsigned int)) |= 0x10000;
 *(amd64_local_APIC_base_address + 0x350/sizeof(unsigned int)) |= 0x10000;
 *(amd64_local_APIC_base_address + 0x360/sizeof(unsigned int)) |= 0x10000;
 *(amd64_local_APIC_base_address + 0x370/sizeof(unsigned int)) |= 0x10000;
}

static unsigned int
read_io_apic_register(register unsigned int const register_number)
{
 *amd64_io_apic_address = register_number;

 return(*(amd64_io_apic_address+16/sizeof(unsigned int)));
}

static void
write_io_apic_register(register unsigned int const register_number,
                       register unsigned int const value)
{
 *amd64_io_apic_address = register_number;

 *(amd64_io_apic_address+16/sizeof(unsigned int)) = value;
}

/*! Initialize one processor. */
static void
init_processor(register const unsigned int processorIndex)
{
 /* Set up the FS and GS bases. */

 /* Set the FS base. */
 wrmsr(0xc0000100, 0);

 /* Set the GS bases. This may look odd but swapgs will swap in the
    user mode GS base. */

 wrmsr(0xc0000102, 0); /* Set the GS kernel base. */
 wrmsr(0xc0000101, (uint64_t) &amd64_CPU_private_table[processorIndex]);

 /* Set up MSRs for system call support. */

 /* Set the legacy sysenter MSRs to illegal values. */
 wrmsr(0x174, 0x3); /* Set the CS selector to be a null selector.
                       This should cause an exception whenever a
                       sysenter is executed. */
 wrmsr(0x175, 0x0);
 wrmsr(0x176, 0x0);

 /* Set STAR. */
 wrmsrl(0xc0000081, 0xffffffff, 0x00030018);

 /* Set LSTAR. */
 wrmsr(0xc0000082, (uint64_t) amd64_syscall_entry_point);

 /* Set CSTAR. */
 wrmsr(0xc0000083, (uint64_t) amd64_syscall_dummy_target);

 wrmsr(0xc0000084, 0x00000300); /* Set the SFMASK. */

 lldt(0);
 ltr(40+processorIndex*16);
 lidt(256*16-1, (uint64_t) amd64_IDT);
}

/*! This function is called from the assembly language portion of the
    bootstrap code after 64-bit mode is invoked. The function brings the
    system to a state where the boot strap processor is initialized. */
void
amd64_multiboot_main(void);

void
amd64_multiboot_main(void)
{
 register uint64_t interruptHandler = (uint64_t) amd64_interrupt_entries;

 /* Create IDT. */
 for(register int i=0; i<256; i++, interruptHandler+=16)
 {
  amd64_IDT[i*2] = (((0xffff0000 & interruptHandler) | 0x8e00) << 32) |
                    (((0xffff & interruptHandler) | (24*0x10000)));
  amd64_IDT[i*2+1] = (interruptHandler>>32) & 0xffffffff;
 }

 /* Create TSSes. */
 static uint32_t TSS[16*28] __attribute__((aligned (16)));

 for (int tssIndex = 0; tssIndex < 16; tssIndex++)
 {
  TSS[tssIndex*28+25] = 0x00680000;
  *((uint64_t *)(&TSS[tssIndex*28+1])) = 0x200000 - 2*4096*tssIndex;
  *((uint64_t *)(&TSS[tssIndex*28+3])) = 0x200000 - 2*4096*tssIndex;
  *((uint64_t *)(&TSS[tssIndex*28+5])) = 0x200000 - 2*4096*tssIndex;

  *((uint64_t *)(&TSS[tssIndex*28+9])) = 0x200000 - 2*4096*tssIndex;
  *((uint64_t *)(&TSS[tssIndex*28+11])) = 0x200000 - 2*4096*tssIndex;
  *((uint64_t *)(&TSS[tssIndex*28+13])) = 0x200000 - 2*4096*tssIndex;
  *((uint64_t *)(&TSS[tssIndex*28+15])) = 0x200000 - 2*4096*tssIndex;
  *((uint64_t *)(&TSS[tssIndex*28+17])) = 0x200000 - 2*4096*tssIndex;
  *((uint64_t *)(&TSS[tssIndex*28+19])) = 0x200000 - 2*4096*tssIndex;
  *((uint64_t *)(&TSS[tssIndex*28+21])) = 0x200000 - 2*4096*tssIndex;

  /* Build TSS descriptor. */
  amd64_BSP_GDT[2*tssIndex+5] =
   (((((((uint64_t) &TSS[tssIndex*28]) & 0xff000000) |
    ((((uint64_t) &TSS[tssIndex*28]) & 0x00ff0000) >>16)) | 0x8900)) << 32) |
    ((((((uint64_t) &TSS[tssIndex*28]) & 0xffff) << 16) |(0x67)));
  amd64_BSP_GDT[2*tssIndex+6] =
   (((uint64_t) &TSS[tssIndex*28])>>32) & 0xffffffff;
 }

 init_processor(0);
}

/*! This function is called after the boot strap processor is fully
    initialized. It is executed using the idle thread stack. The
    function finalizes the system initialization. */
void
amd64_system_start(const unsigned long boot_stack_pointer
                   /*!< Points to the top of the stack used by the
                        boot strap code. */);

void
amd64_system_start(const unsigned long boot_stack_pointer)
{
 /* Initialize IO APIC disabling all interrupts. */
 {
  register int i;
  for(i=0; i<24; i++)
  {
   register unsigned int const io_apic_low_bits =
    read_io_apic_register(0x10 + 2*i);
   write_io_apic_register(0x10 + 2*i, io_apic_low_bits | 0x10000);
  }
 }

 /* Route NMIs and 8259 interrupts through the APIC. */
 out8(0x22, (uint8_t)0x70);
 out8(0x23, (uint8_t)1);

 /* Copy the application processor bootstrap code into place. */
 {
  register char*       dst = (char*) 0x10000;
  register const char* src = amd64_start_application_processor;

  for(; src<amd64_start_application_processor_end;)
  {
   *dst++=*src++;
  }
 }

 /* Bootstrap all processors. */
 {
  register unsigned int processor_index;

  initialize_APIC();
  number_of_initialized_CPUs=1;

  for(processor_index = 1;
      processor_index<amd64_number_of_available_CPUs;
      processor_index++)
  {
   /* Set up information to be sent to the application processor boot strap. */
   amd64_AP_processor_index = processor_index;

   /* Application processor. Send IPI to wake it up. */

   /*! \todo The SIPI procedure is not bullet proof. It works with bochs but
      to be fully working on all CPU types and revisions there should be a
      timeout and a second try if the first does not work. */

   /* Send INIT command. */
   *(amd64_local_APIC_base_address+0x310/4) =
    amd64_CPU_private_table[processor_index].APICId<<(56-32);
   *(amd64_local_APIC_base_address+0x300/4) = 0x510;

   /* Pause for a moment, using busy wait and CPU0's APIC timer. */
   {
    /* Set divider to 2. */
    *(amd64_local_APIC_base_address+0x3e0/4) = 0x00;

    /* Set initial count */
    *(amd64_local_APIC_base_address+0x380/4) = 1000000;

    while (*(amd64_local_APIC_base_address+0x390/4) > 0);
   }

   /* Send SIPI. */
   *(amd64_local_APIC_base_address+0x300/4) = 0x610;

   /* Spin until the application processor is woken up. */
   while((processor_index+1) != number_of_initialized_CPUs);
  }
 }

 /* Initialize IO-APIC interrupts. */
 {
  register int timer_gsi = amd64_pic_interrupt_map[0];
  /* Send timer interrupts to all cpus. */
  write_io_apic_register(0x11 + timer_gsi*2, 0xff000000);
  write_io_apic_register(0x10 + timer_gsi*2, 0x00000020);
 }

 /* Go to user space and execute the first process. */
 //kcreateprocess(0);

 //fxsave(getActiveContext()->fp_context);
 kcreateprocess(0);
 returnToUserLevel(getActiveContext(), 0);
}

void
amd64_AP_init(void);

void
amd64_AP_init(void)
{
 init_processor(amd64_AP_processor_index);
 initialize_APIC();
 number_of_initialized_CPUs++;

 /* Add code that allows the AP to run the scheduler. */

 /* Remove the following while loop once you have implemented proper scheduling
    for the AP. */
 while(1)
 {
  sti();
  hlt();
  cli();
 }
}


/*! Helper function for kalloc and kfree */
char isValid(char* ptr) {
  blockPtr block= (blockPtr)(ptr-BLOCKSIZE);
  if( block->full==1 &&
      (block->next==0 || (block->next >= base && block->next <= (blockPtr)(((char*)base)+ALLOCATE_SIZE))) &&
      (block->prev==0 || (block->prev >= base && block->prev <= (blockPtr)(((char*)base)+ALLOCATE_SIZE))) &&
      block->size<=ALLOCATE_SIZE )
    return 1;
  return 0;
}

/*! Helper function for kalloc and kfree */
blockPtr merge(blockPtr first, blockPtr second) {
  first->next=second->next;
  first->size=((char*)second)- ((char*)first) + second->size;
  if(second->next)
    second->prev=first;
  return first;
}


/*! Helper function for kalloc and kfree */
blockPtr findPlace(uint64_t size) {
  blockPtr _base=base;
  last=0;

  while(_base && (_base->full ||  _base->size < size)) {
    last=_base;
    _base=_base->next;
  }
  return _base;
}

/*! Helper function for kalloc and kfree */
blockPtr extendMemory(uint64_t size) {
  blockPtr new=0;
  if(last!=0) {
    new=(blockPtr)((char*)last + BLOCKSIZE + last->size);
    last->next=new;
  }
  else {
    new=(blockPtr)amd64_lowest_available_physical_memory;
    base=new;
  }
  new->prev=last;/* if last is null then it holds. if last isn't full it still holds*/
  new->size = size;
  new->next=0;
  return new;
}

/*! Helper function for kalloc and kfree */
blockPtr splitMemory(blockPtr current ,uint64_t size) {
  blockPtr smallPart;

  if(current->size >= size + BLOCKSIZE + 4) {
    smallPart=(blockPtr)((char*)current + BLOCKSIZE + size);
    smallPart->next=current->next;
    smallPart->next->prev=smallPart;
    current->next=smallPart;
    smallPart->prev=current;
    smallPart->full=0;
    smallPart->size=current->size - BLOCKSIZE - size;
    current->size = size;
    return current;
  }
  else {

    /* ?? */
    return current;
  }
}

/*! Allocates a memory block.
    \return an address to the memory block or an error code if
            the allocation was not successful. */
long
kalloc(const register uint64_t length)
{
	  register uint64_t t= (length & 0x1f);
	  uint64_t size = t==0 ? length : length + 32 - t;
	  blockPtr p = findPlace(size), found;
	  if(p == 0) { /*checking if it fits */
	    register int a = ALLOCATE_SIZE - (last-base)-BLOCKSIZE;
	    if(last!=0)
	      a-=last->size+BLOCKSIZE;
	    if(size > a)
	      return ERROR;
	   found= extendMemory(size);
	  }
	  else {
	    found = splitMemory(p,size);
	  }
	  found->full=1;
	  return (long)((char *)found + BLOCKSIZE);

}

/*! Frees a previously allocated a memory block.
    \return ALL_OK if successful or an error code if
            the free was not successful. */
long
kfree(const register uint64_t address)
{
	  blockPtr freed,thePrev,theNext,temp;
	  if(!isValid((char*)address)) {
	    return ERROR;
	  }

	  freed=(blockPtr)(((char*)address)-BLOCKSIZE);
	  theNext= freed->next;
	  thePrev= freed->prev;

	  if(theNext && !theNext->full){
	    temp=merge(freed, theNext);
	    temp->full=1;
	    kfree(((uint64_t)temp)+BLOCKSIZE);
	  }
	  else if(theNext && theNext->full){
	    if(thePrev && !thePrev->full){
	      temp=merge(thePrev,freed);
	      theNext->prev=temp;
	    }
	    else {
	      freed->full=0;
	    }
	  }
	  else {
	    if(!thePrev) {
	      base=0;
	    }
	    else {
	      thePrev->next=0;
	      if(!thePrev->full) {
		thePrev->full=1;
		kfree(((uint64_t)thePrev)+BLOCKSIZE);
	      }
	    }
	  }
	  return ALL_OK;
}


/*! Terminates the caller process. */
void kterminate()
{
	struct process_entry terminated = pop_process_queue(); /* Pops queue.*/
	kfree((uint64_t) terminated.context); /* Deallocate terminated context. */
	kfree(terminated.memory_location); /* Deallocate segments in memory. */
	if(is_empty_process_queue())
	{
		/* Queue is empty. There is no other process to assign CPU.*/
		kprints("the last process is terminated! \n");
		return;
	}
	setActiveContext(top_process_queue().context); /* Set context of top-most element in process queue as active context. */

}

/*! Creates a process and pushes it to the process stack.
 * \return status of operation. */
unsigned long kcreateprocess(uint64_t rdi)
{
	const struct Elf64_Ehdr*  elfImage = ELF_images[rdi];
	struct process_entry new_process;
	uint64_t entry_point = 0, memory_location = 0 ;
	copy_ELF(elfImage, &entry_point, &memory_location); /* Parse ELF image. */
	if(entry_point==0 || memory_location == 0)
	{
		/* Invalid ELF File. */
		kprints("\nit is an error. copy_ELF didn't work well! \n");
		return ERROR;
	}
	/* Allocate memory for new context. */
	struct AMD64Context * newContext = (struct AMD64Context*)kalloc(sizeof(struct AMD64Context));
	if(newContext==(struct AMD64Context*)ERROR) /* Kmalloc has failed! */
		return ERROR;

	/* Set rflags and rip registers of new context. */
	newContext->rflags=0x200;//try 200
	newContext->rip = entry_point;

	/*  Initialize process struct fields. */
	new_process.context=newContext;
	new_process.memory_location=memory_location; /* We need it to be able to free it during termination. */
	new_process.id=rdi; /* Process id is index of its ELF image */
	new_process.state=READY; /* Initially READY */

	push_back_process_queue(new_process); /* Push new process to the back of the process queue */

	if(top_process==(struct process_queue_element * ) 0) /* Push_back operation must have failed. */
		return ERROR;

	/* When recently create process is the only process executed by operating system
	 * we need to start executing it by setting active context as new process's context.
	 * Then we also set state info accordingly.
	 */
	if(top_process==back_process)
	{
		setActiveContext(newContext);
		top_process->element.state=RUNNING;
	}

	return ALL_OK;
}
