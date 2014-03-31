#include "globals.h"

/*! Round robin scheduling algorithm */
void scheduler()
{
	struct AMD64Context * active_context;
	if(is_empty_process_queue()) /* If queue is empty, do nothing. */
	{
		return;
	}
	else
	{
		if(top_process!=back_process){/* If correct, there are waiting processes. We need to switch context. */
			push_back_process_queue(pop_process_queue()); /* Remove first, push it back */
			back_process->element.state=READY;  /* Set bottom element's state as READY. */
			top_process->element.state=RUNNING; /* Set first  element's state as RUNNING. */
			active_context=top_process_queue().context;
			setActiveContext(active_context); /* Context switch. */
		}
	}
}
