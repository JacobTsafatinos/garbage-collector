/* Test operations on a linked list.
 * Call the garbage collector regularly.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "list.h"
#include "gc.h"
#include "gc_list.h"

//instantiate ll
static List *ll = NULL;

void mark_list(void *);

//struct_test compares the addresses of the test struct and the mem_chunk
//to make sure they're the same - returns 1 if they're the same. Otherwise returns 0
int struct_test() {

    struct mem_chunk *head_ptr = head_chunk;
    List *list_ptr = ll;
    int matched = 1;

    while(head_ptr!=NULL && list_ptr != NULL){

        printf("list_ptr is %s\n", (char*)&list_ptr);
        printf("head_ptr is %s\n", (char*)&(head_ptr->address));

        if (list_ptr != head_ptr->address){

            matched = 0;

        }

        head_ptr = head_ptr->next;
        list_ptr = list_ptr->next;
    }

    return matched;
}

int main(int argc, char **argv) {

    //clear the logfile
    FILE *logfile;
    logfile = fopen(LOGFILE, "w");
    fclose(logfile);

    //add 9 nodes
    ll = add_node(ll, 1);
    ll = add_node(ll, 2);
    ll = add_node(ll, 3);
    ll = add_node(ll, 4);
    ll = add_node(ll, 5);
    ll = add_node(ll, 6);
    ll = add_node(ll, 7);
    ll = add_node(ll, 8);
    ll = add_node(ll, 9);


    //call mark_and_sweep on the nodes, expext 9 nodes remaining
    mark_and_sweep(ll, mark_list);

    printf ("returned value is: %d\n", struct_test());

    //remove the middle node
    ll = remove_node(ll, 5);

    //call mark_and_sweep, expect all nodes but the middle
    mark_and_sweep(ll, mark_list);

    printf ("returned value is: %d\n", struct_test());

    //remove the first node
    ll = remove_node(ll, 1);

    //call mark_and_sweep, expect all but first node
    mark_and_sweep(ll, mark_list);
    
    printf ("returned value is: %d\n", struct_test());

    //remove the last node
    ll = remove_node(ll, 9);

    //call mark_and_sweep, expect all but last node;
    mark_and_sweep(ll, mark_list);

    printf ("returned value is: %d\n", struct_test());

    //remove all the nodes
    ll = remove_node(ll, 2);
    ll = remove_node(ll, 3);
    ll = remove_node(ll, 4);
    ll = remove_node(ll, 6);
    ll = remove_node(ll, 7);
    ll = remove_node(ll, 8);

    //call mark_and_sweep, expect no nodes
    mark_and_sweep(ll, mark_list);
    printf ("returned value is: %d\n", struct_test());

    return 0;

}