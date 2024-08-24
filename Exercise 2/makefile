OBJS	= main.o second_solver.o maze.o priority_queue/heap.o
SOURCE	= main.c second_solver.c maze.c priority_queue/heap.c
HEADER	= second_solver.h maze.h priority_queue/heap.h
OUT	= out
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c

second_solver.o: second_solver.c
	$(CC) $(FLAGS) second_solver.c

maze.o: maze.c
	$(CC) $(FLAGS) maze.c

priority_queue/heap.o: priority_queue/heap.c
	$(CC) $(FLAGS) -o priority_queue/heap.o priority_queue/heap.c 


clean:
	rm -f $(OBJS) $(OUT)