PROG = graph
CC=g++
CFLAGS = -c -Wall

all: $(PROG)

$(PROG): test.o graph.o
	$(CC) test.o graph.o -o $(PROG)

test.o: test.cpp
	$(CC) $(CFLAGS) test.cpp
graph.o: graph.cpp graph.h
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -f *.o $(PROG)
run:
	./$(PROG)
runv:
	valgrind ./$(PROG)