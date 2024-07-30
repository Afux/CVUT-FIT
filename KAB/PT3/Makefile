CC = g++
CFLAGS = -ggdb3
LDFLAGS = -lcrypto  

all: test
run:
	#run your tests here...
test: sample.o
	$(CC) -o $@ $< $(LDFLAGS)
%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

