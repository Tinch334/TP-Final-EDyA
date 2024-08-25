OBJS = main.o maze.o first_solver.o stack/glist.o stack/stack.o
SOURCE = main.c maze.c first_solver.c stack/glist.c stack/stack.c
HEADER = maze.h first_solver.h stack/glist.h stack/stack.h
OUT	= out
CC = gcc
FLAGS = -lm -g -c -Wextra -pedantic

all: $(OBJS)
	$(CC) -g $(OBJS) -lm -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c

maze.o: maze.c
	$(CC) $(FLAGS) maze.c 

first_solver.o: first_solver.c
	$(CC) $(FLAGS) first_solver.c  

#There's probably a proper way to make the targets leave the files where they are supposed to be, but I haven't found it.
stack/glist.o: stack/glist.c
	$(CC) $(FLAGS) -o stack/glist.o stack/glist.c 

stack/stack.o: stack/stack.c
	$(CC) $(FLAGS) -o stack/stack.o stack/stack.c 


clean:
	rm -f $(OBJS) $(OUT)