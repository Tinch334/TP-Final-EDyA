OBJS	= main.o second_solver.o maze.o utils/custom_string.o utils/gdlist.o utils/queue.o
SOURCE	= main.c second_solver.c maze.c utils/custom_string.c utils/gdlist.c utils/queue.c
HEADER	= second_solver.h maze.h utils/custom_string.h utils/gdlist.h utils/queue.h
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

utils/gdlist.o: utils/gdlist.c
	$(CC) $(FLAGS) -o utils/gdlist.o utils/gdlist.c

utils/queue.o: utils/queue.c
	$(CC) $(FLAGS) -o utils/queue.o utils/queue.c


clean:
	rm -f $(OBJS) $(OUT)