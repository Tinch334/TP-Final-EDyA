OBJS	= main.o second_solver.o maze.o utils/custom_string.o utils/glist.o utils/stack.o
SOURCE	= main.c second_solver.c maze.c utils/custom_string.c utils/glist.c utils/stack.c
HEADER	= second_solver.h maze.h utils/custom_string.h utils/glist.h utils/stack.h
OUT	= out
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -lm -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c

second_solver.o: second_solver.c
	$(CC) $(FLAGS) second_solver.c

maze.o: maze.c
	$(CC) $(FLAGS) maze.c

utils/custom_string.o: utils/custom_string.c
	$(CC) $(FLAGS) -o utils/custom_string.o utils/custom_string.c

utils/glist.o: utils/glist.c
	$(CC) $(FLAGS) -o utils/glist.o utils/glist.c 

utils/stack.o: utils/stack.c
	$(CC) $(FLAGS) -o utils/stack.o utils/stack.c


clean:
	rm -f $(OBJS) $(OUT)