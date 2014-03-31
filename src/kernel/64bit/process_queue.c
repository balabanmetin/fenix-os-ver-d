#include "globals.h"

struct process_queue_element *  top_process=0;
struct process_queue_element * back_process=0;



uint64_t is_empty_process_queue()
{
	return (top_process == (struct process_queue_element *) 0 );
}

void push_back_process_queue(struct process_entry process)
{
	/* Allocate space for element. */
	struct process_queue_element * new_process =
   (struct process_queue_element * ) kalloc(sizeof( struct process_queue_element)) ;
	if(new_process==(struct process_queue_element * )ERROR)
	{
		while(1) kprints("push_queue_process couldn't claim memory for new process!");
	}

	new_process->element=process;

	/* If queue is empty, we need to initialize both top and last element pointers. */
	if(is_empty_process_queue())
	{
		back_process = new_process;
		top_process  = new_process;
		return;
	}
	else
	{
		back_process->next=new_process;
		back_process 	  =new_process;
	}

}

struct process_entry top_process_queue()
{
	/* Stack shouldn't be empty */
	if(is_empty_process_queue()){
		while(1)
		{
			kprints("kernel panic: process queue corrupted!");
		}
	}
	return top_process->element;
}
struct process_entry pop_process_queue()
{
	struct process_entry temp;
	struct process_queue_element * deleted;
	temp.context=(struct AMD64Context*) 0;
	/* Stack shouldn't be empty */
	if(is_empty_process_queue()){
		while(1)
		{
			kprints("kernel panic: process queue corrupted!");
		}
	}
	deleted=top_process;
	temp=top_process->element;
	/* If there is a single element, we need to set both top and last element pointers to zero. */
	if(top_process==back_process){
		top_process =(struct process_queue_element *)0;
		back_process=(struct process_queue_element *)0;
	}
	else{
		top_process = top_process->next;
	}
	/* Reclaim memory. */
	kfree((uint64_t)deleted);
	return temp;
}
