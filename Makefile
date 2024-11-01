WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla -Werror 
GCC = gcc -std=c99 -g $(WARNING) $(ERROR)
VAL = valgrind --tool=memcheck --log-file=memcheck.txt --leak-check=full --show-leak-kinds=all --verbose

SRCS = main.c a6.c
OBJS = $(SRCS:%.c=%.o)

a6: $(OBJS)
	$(GCC) $(OBJS) -o a6 -lm

.c.o:
	$(GCC) -c $*.c

test_file: a6
	$(VAL) --track-origins=yes ./a6 3.po test.pr test.dim test.pck

8_po: a6
	$(VAL) --track-origins=yes ./a6 8.po test.pr test.dim test.pck

clean: # remove all machine generated files
	rm -f a6 *.o output? *~ memcheck.txt 