CC := g++
CFLAGS := -Wall -std=c++11

prof : prof.o
	$(CC) $^ -o $@

prof.o : prof.cc
	$(CC) $(CFLAGS) -c -o $@ $^

clean :
	rm -rf prof.o prof
