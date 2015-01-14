/* Read and execute a list of operations on a linked list.
 * Periodically call the garbage collector.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "list.h"
#include "gc.h"
#include "gc_list.h"
#include <signal.h>

#define MAX_LINE 128
#define ADD_NODE 1
#define DEL_NODE 2
#define PRINT_LIST 3

//declare ll as a static var to be called in sig_h
static List *ll = NULL;

void mark_list(void *);

//signal handler
void sig_h(int signum)
{
    if (signum == SIGUSR1)
    {
        mark_and_sweep(ll, mark_list);
    }
}


int main(int argc, char **argv) {
    //clear the logfile
    FILE *logfile;
    logfile = fopen(LOGFILE, "w");
    fclose(logfile);

    char line[MAX_LINE];
    char *str;

    if(argc != 2) {
        fprintf(stderr, "Usage: do_trans filename\n");
        exit(1);
    }

    FILE *fp;
    if((fp = fopen(argv[1], "r")) == NULL) {
        perror("fopen");
        exit(1);
    }

    int count = 1;
    signal(SIGUSR1, sig_h);
    while(fgets(line, MAX_LINE, fp) != NULL) {
        char *next;
        int value;
        int type = strtol(line, &next, 0);

        switch(type) {
            break;
            case ADD_NODE :
                 //initialize the signal set
                ;
                sigset_t sigs;
                sigemptyset(&sigs);
                //addiong SIGINT to the set
                sigaddset(&sigs, SIGUSR1);
                
                //add signals to the blocked set
                sigset_t old_sigs;

                if(sigprocmask(SIG_BLOCK, &sigs, &old_sigs) != 0) {
                    exit(1);
                }
                value = strtol(next, NULL, 0);
                ll = add_node(ll, value);
                value = strtol(next, NULL, 0);
                ll = add_node(ll, value);
                if(sigprocmask(SIG_SETMASK, &old_sigs, NULL) != 0) {
                    exit(1);
                }
                break;
            case DEL_NODE :
                value = strtol(next, NULL, 0);
                ll = remove_node(ll, value);
                break;
            case PRINT_LIST :
                str = tostring(ll);
                printf("List is %s\n", str);
                break;
            default :
                fprintf(stderr, "Error: bad transaction type\n");

        }
    
        
        if(count % 10 == 0) {
            mark_and_sweep(ll, mark_list);
            // You might want to add something here to
            // make your program pause long enough to see what it
            // is doing. In the commented out code, we wait for 
            // any user input before continuing.
 
            // char check[MAX_LINE];
            // fgets(check, MAX_LINE, stdin);
        }       
        count++;

    }
    return 0;
}
