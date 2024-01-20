CC = g++
CFLAGS = -Wall
DEPS = lib2.h
OBJ = tracker.o terminal.o functionality.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

tracker: $(OBJ)
	$(CC) $(CFLAGS) -pthread -o $@ $^ 

clean:
	rm -rf *o tracker