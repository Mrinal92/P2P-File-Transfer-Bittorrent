CC = g++
CFLAGS = -Wall
DEPS = lib.h
OBJ = Client.o Encrypt.o terminal.o peerRequests.o transfer.o functionality.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

client: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lssl -lcrypto -pthread 
 
clean:
	rm -rf *o Client