/* Copyright (c) 1997-2012, FenixOS Developers
   All Rights Reserved.

   This file is subject to the terms and conditions defined in
   file 'LICENSE', which is part of this source code package.
 */

/*! \file entry_routines.cpp This file holds implementations of interrupt
    routines. These fuctions are called directly from the low-level assembly
    language interrupt code.
 */

#include "globals.h"
#include "instruction_wrappers.h"

void static
handle_interrupt(const int interrupt)
{
 /* Select a handler based on interrupt source. */
 static uint64_t time_clicks;

 switch(interrupt)
 {
  case 32:
  {
   /* PIT interrupt occurred. */
	  time_clicks++;
	  if(((time_clicks>>3) & 1) != 1) // 40 ms = 5 ms * 2^3
		  scheduler();
	  break;
  }

  case 240:
  case 241:
  case 242:
  case 243:
  case 244:
  case 245:
  case 246:
  case 247:
  case 248:
  case 249:
  case 250:
  case 251:
  case 252:
  case 253:
  case 254:
  {
   /* Reserved for IPIs. */
   break;
  }

  case 255:
  case 39:
  {
   /* Spurious interrupt occurred. This could happen if we spend too long
      time with interrupts disabled. */
   break;
  }

  default:
  {
   /* Unknown interrupt. Not good. */
   amd64_halt();
  }
 }
}

/*! This function is called if an interrupt occurs in user mode.
    WARNING: This function never returns. */
void
amd64_handle_interrupt_user(int interrupt /*!< The interrupt vector number. */)
{
 /* Get the currently active context. */
 struct AMD64Context * active_context = getActiveContext();

 active_context -> interrupt_context = 1;

 /* Save the floating point context. */
 fxsave(active_context->fp_context);

 handle_interrupt(interrupt);

 /* Acknowledge interrupt so that new interrupts can be sent to the CPU. */
 if (interrupt >= 32)
 {
  /* Do an EOI procedure on the local APIC. */

  /* Acknowledge the interrupt. */
  *(amd64_local_APIC_base_address + 0xb0/sizeof(unsigned int)) = 0;
  active_context = getActiveContext();
 }

 /* Return to user space. */
 returnToUserLevel(active_context, 1);

 /* Here to make the compiler happy. This function does not return. */
 while(1);
}

/*! This function is called if an interrupt occurs in supervisor mode. */
void
amd64_handle_interrupt_super(int interrupt /*!< The interrupt vector number. */)
{
 handle_interrupt(interrupt);

 /* Acknowledge interrupt so that new interrupts can be sent to the CPU. */
 if (interrupt >= 32)
 {
  /* Do an EOI procedure on the local APIC. */

  /* Acknowledge the interrupt. */
  *(amd64_local_APIC_base_address + 0xb0/sizeof(unsigned int)) = 0;
 }
}

/*! This function is called if an exception occurs in user mode.
    WARNING: This function never returns.  */
void
amd64_handle_exception_user(int exc /*!< The exception vector number. */)
{
 /* We do not handle exceptions. */

 amd64_halt();

 /* Here to make the compiler happy. This function does not return. */
 while(1);
}

/*! This function is called if an interrupt occurs in supervisor mode. */
void
amd64_handle_exception_super(int exc /*!< The exception vector number. */)
{
 /* We do not handle exceptions. */

 amd64_halt();

}

/*! This function is called if an exception with error code occurs in user
    mode. WARNING: This function never returns. */
void
amd64_handle_exception_w_error_user(int exc /*!<The exception vector number.*/,
                                    unsigned int code /*!< The error code. */)
{
 /* We do not handle exceptions. */

 amd64_halt();

 /* Here to make the compiler happy. This function does not return. */
 while(1);
}

/*! This function is called if an exception with error code occurs in
    supervisor mode. */
void
amd64_handle_exception_w_error_super(int exc /*!<The exception vector number.*/,
                                     unsigned int code /*!< The error code. */)
{
 /* We do not handle exceptions. */

 amd64_halt();
}

/*! This function is called if a NMI occurs in user mode.
    WARNING: This function never returns. */
void
amd64_handle_nmi_user(int dummy /*!< Not used. */)
{
 /* We do not handle exceptions. */

 amd64_halt();

 /* Here to make the compiler happy. This function does not return. */
 while(1);
}

/*! This function is called if a NMI occurs in supervisor mode. */
void
amd64_handle_nmi_super(int dummy  /*!< Not used. */)
{
 /* We do not handle exceptions. */

 amd64_halt();
}

/*! This function is called when a system call is invoked.
    WARNING: This function never returns.  */
void
amd64_enter_kernel(void)
{
 /* Get the currently active context. */
 struct AMD64Context * active_context = getActiveContext();

 active_context -> interrupt_context = 0;

 /* Save the floating point context. */
 fxsave(active_context->fp_context);

 switch (active_context->rax)
 {
  /* System call implementations goes here. Preferably they use architecture
     independent functions. Hence, what is left here is glue code which 
     interacts with the active context. */

  case SYSCALL_VERSION:
  {
   active_context->rax = 0;
   break;
  }

  case SYSCALL_PRINTS:
  {
   kprints((const char *) active_context->rdi);
   active_context->rax = ALL_OK;
   break;
  }

  case SYSCALL_PRINTHEX:
  {
   kprinthex(active_context->rdi);
   active_context->rax = ALL_OK;
   break;
  }

  case SYSCALL_DEBUGGER:
  {
   amd64_enter_debugger();
   active_context->rax = ALL_OK;
   break;
  }

  case SYSCALL_ALLOCATE:
  {
   active_context->rax = kalloc(active_context->rdi);
   break;
  }

  case SYSCALL_FREE:
  {
   active_context->rax = kfree(active_context->rdi);
   break;
  }

  case SYSCALL_TERMINATE:
    {
  	  kterminate();
  	  active_context=getActiveContext();
  	  break;
    }
    case SYSCALL_CREATEPROCESS:
    {
  	  active_context->rax=kcreateprocess(active_context->rdi);//return values
  	  if(active_context->rax==ALL_OK)
  		  active_context=getActiveContext();
  	  break;
    }

  default:
  {
   /* Hmm. System call which is not implemented. Not good. */
   amd64_halt();
  }
 }

 /* Return to user space. */
 returnToUserLevel(active_context, 0);
 /* NB: returnToUserLevel can return in some cases. This could be handy
    for scheduling algorithms. */

 /* Here to make the compiler happy. This function does not return. */
 while(1);
}
