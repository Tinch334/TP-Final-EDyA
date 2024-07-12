OBJS	= main.o maze.o stack/glist.o stack/stack.o
SOURCE	= main.c maze.c stack/glist.c stack/stack.c
HEADER	= maze.h stack/glist.h stack/stack.h
OUT	= out
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c 

maze.o: maze.c
	$(CC) $(FLAGS) maze.c 

glist.o: stack/glist.c
	$(CC) $(FLAGS) stack/glist.c 

stack.o: stack/stack.c
	$(CC) $(FLAGS) stack/stack.c 


clean:
	rm -f $(OBJS) $(OUT)