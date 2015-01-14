#include <stdio.h>
#include <stdlib.h>
#include "fstree.h"
#include "gc.h"
#include "gc_trees.h"

/* Recursively traverse a tree and call mark_one on it. 
 */
void mark_tree(void *tree_root){

	//create a Fstree to iterate over
	Fstree *fpt = tree_root;

	if(mark_one(fpt) != 1){

		mark_one(fpt->name);
	    Link *cur_link = fpt->links;

	    while(cur_link != NULL){

	        mark_tree(cur_link->fptr);
	        mark_one(cur_link);	

	        cur_link = cur_link->next;
    	}
	}
}