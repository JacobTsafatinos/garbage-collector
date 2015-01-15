# garbage-collector
This is a trial garbage collector to track and reclaim memory for linked lists and file system trees. Written in C, allocated space is tracked when 'gc_malloc' is called, and removed when 'mark_and_sweep' is called.

#Files
gc.c: Contains the garbage collector (gc_malloc and mark_and_sweep)
mktrans.c: Generate a random series of add, delete, and print operations on a linked list.
do_trans.c: Read and execute a list of operations on a linked list. Periodically call the garbage collector.
do_fstree.c: Performs a series of operations on the fstree data structure.
listtest.c: Tests operations on a linked list

#Testing
I've included a simple test program "listtest". It adds and removes a nodes in different sequences in order to demonstrate the garbage collection with different cases. To compile, first call make and then call listtest.

