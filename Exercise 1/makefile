OBJS = main.o maze.o first_solver.o utils/glist.o utils/stack.o utils/hash_table.o
SOURCE = main.c maze.c first_solver.c utils/glist.c utils/stack.c utils/hash_table.c
HEADER = maze.h first_solver.h utils/glist.h utils/stack.h utils/hash_table.h
OUT	= out
CC = gcc
FLAGS = -lm -g -c -Wextra -pedantic -Wconversion -Wall

all: $(OBJS)
	$(CC) -g $(OBJS) -lm -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c

maze.o: maze.c
	$(CC) $(FLAGS) maze.c 

first_solver.o: first_solver.c
	$(CC) $(FLAGS) first_solver.c  

#There's probably a proper way to make the targets leave the files where they are supposed to be, but I haven't found it.
utils/glist.o: utils/glist.c
	$(CC) $(FLAGS) -o utils/glist.o utils/glist.c 

utils/stack.o: utils/stack.c
	$(CC) $(FLAGS) -o utils/stack.o utils/stack.c

utils/hash_table.o: utils/hash_table.c
	$(CC) $(FLAGS) -o utils/hash_table.o utils/hash_table.c


clean:
	rm -f $(OBJS) $(OUT)