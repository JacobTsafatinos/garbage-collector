#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc.h"
#include "gc_list.h"
#include "list.h"

/* Traverse the list and set
 * mem_chunk to used if it reaches it
 */
void mark_list(void *list_head){

	//create a temp list to iterate through
	List *temp = list_head;
	while(temp != NULL) {
		mark_one(temp);
		temp = temp->next;
    }	
}