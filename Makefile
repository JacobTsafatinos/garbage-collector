LOGFILE='"gc.log"'
FLAGS = -DLOGFILE=${LOGFILE} -Wall -g

all : mktrans do_trans do_fstree listtest

do_trans : do_trans.o list.o gc.o gc_list.o
	gcc ${FLAGS} -o $@ $^

listtest : listtest.o list.o gc.o gc_list.o
	gcc ${FLAGS} -o $@ $^

mktrans : mktrans.o list.o gc.o gc_list.o
	gcc ${FLAGS} -o $@ $^

do_fstree : do_fstree.o fstree.o gc.o gc_trees.o
	gcc ${FLAGS} -o $@ $^

%.o : %.c
	gcc ${FLAGS} -c $<

do_trans.o : list.h gc.h gc_list.o
mktrans.o : list.h
list.o : list.h gc.h
fstree.o : fstree.h
do_fstree.o :fstree.h gc_trees.o
gc.o : gc.h
gc_list.o : gc_list.h gc.h
gc_trees.o : gc_trees.h gc.h


clean :
	rm *.o mktrans do_trans do_fstree