#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc.h"
#include "gc_list.h"
#include <signal.h>
#include <unistd.h>

//Global pointer to keep track of the head of mem_chunk
struct mem_chunk *head_chunk;

//Declare a count of deleted memory whenever something is freed by the garbage collector
static int freed = 0;

//Declare a count of memory allocated whenever gc_malloc gets called
static int mallocked =0;

//Declare a count of number of elements in gc before sweep
static int before_sweep = 0;

//Declare a count for each log
static int logcount = 0;
//Declare a debug value to change in order to debug
static int debug = 0;


/* keeps track of dynamically allocated memory by wrapping around malloc
*/
void *gc_malloc(int nbytes) {
	struct mem_chunk *new_node;
	
	//malloc space for new_node and check if it's NULL
	if ((new_node = malloc(sizeof(struct mem_chunk))) == NULL){
		perror("malloc");
		exit(1);
	}
	
	//malloc space for the address of the struct
	if ((new_node->address = malloc(nbytes)) == NULL){
		perror("malloc");
		exit(1);
	}

	new_node->in_use = USED;

	//Checking the case where there is nothing in head_chunk
	if(head_chunk == NULL) {
		new_node->next = NULL;
		head_chunk = new_node;
	}
	else{
		new_node->next = head_chunk;
		head_chunk = new_node;
	}

	//debug statement to see the memory adresses
	if (debug) {
		printf("Memory allocated at:%s\n", (char*)&(new_node->address));
	}

	mallocked++;
	return new_node->address;
}


/* Collects the garbage. Goes through the data structure
 * marking the memory, then frees space of unused memory. 
 */
void mark_and_sweep(void *obj, void (*mark_obj)(void *)){
	
	logcount++;

	FILE * fout;
	fout = fopen(LOGFILE, "a");

	//create  a temp mem_chunk to iterate through
	struct mem_chunk *temp = head_chunk;
	
	//reset the mem_chunk list, setting in_use to NOT_USED
	while(temp != NULL){
		temp->in_use = NOT_USED;
		temp = temp->next;
	}

	//mark the data structure	
	(*mark_obj)(obj);

	//create a temp mem_chunk to sweep
	struct mem_chunk *temp_sweep;
	
	//checks if the head is marked or not and frees space accordingly
	if(head_chunk != NULL && head_chunk->in_use == NOT_USED) {

		temp_sweep = head_chunk;
		free(head_chunk->address);

		head_chunk = head_chunk->next;
		free(temp_sweep);
		freed++;
	}

	//set temp_sweep to the head_chunk
	temp_sweep = head_chunk;

	//create a temporary holder that stores the value of temp_sweep
	struct mem_chunk *temp_holder = temp_sweep;

	//goes through the mem_chunk object and checks whats been
	//marked, then frees space if it's not marked
	while(temp_sweep != NULL && temp_sweep->next != NULL) {

		if(temp_sweep->next->in_use == NOT_USED){

			temp_holder = temp_sweep->next;
			temp_sweep->next = temp_sweep->next->next;

			free(temp_holder->address);
			free(temp_holder);

			freed++; 	
	    }

	    temp_sweep = temp_sweep->next;
	    temp_holder = temp_sweep;
	}


	
	struct mem_chunk *temptemp = head_chunk;
	while (temptemp != NULL) {
		before_sweep++;
		temptemp = temptemp->next;
	}

	//prints to log how many things have been added or deleted
	fprintf(fout, "Log Number: %d\n", logcount);
	fprintf(fout, "Number of elements added: %d\n", mallocked);
	fprintf(fout, "Number of elements deleted: %d\n", freed);
	fprintf(fout, "Number of elements before sweep: %d\n", before_sweep);

	//reset the values for freed and mallocked
	before_sweep = 0;
	freed = 0;
	mallocked = 0;
	fclose(fout);

	if(debug){
		struct mem_chunk *temptemp = head_chunk;
		while (temptemp != NULL) {
			printf("%s\n", (char*) &(temptemp->address));
			temptemp = temptemp->next;
		}
	}
}

/* Marks an object if it's memory is being pointed to by vptr
 * and returns 1 if the memory chunk is in use, otherwise
 * returns 0
 */
int mark_one(void *vptr){

	if(debug)
	{
		printf("Marking!\n");
	}

	//set a default return value
	int ret = 0;

	//create a temp mem_chunk to iterate through
	struct mem_chunk *temp = head_chunk;

	//iterate through the mem_chunk
	while(temp != NULL){

		//if vptr points to the mem_chunk address mark it
		if(temp->address == vptr) {
			//mark it 1 if it's used
			if(temp->in_use == USED){
				ret = 1;
			}
			//mark it 0 if not used
			else{	
				ret = 0;
			}

			temp->in_use = USED;
		}

		temp = temp->next;
    }
    
    return ret;
};