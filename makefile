P = btrfrsc
CFLAGS = -g -Wall
LDLIBS = -ltsk

btrfrsc : btrfrsc.o Guid.o
	g++ -o $(P) btrfrsc.o Guid.o $(CFLAGS) $(LDLIBS)

btrfrsc.o : btrfrsc.cpp Guid.h
	g++ -c btrfrsc.cpp $(CFLAGS) $(LDLIBS)

Guid.o : Guid.cpp Guid.h
	g++ -c Guid.cpp $(CFLAGS) $(LDLIBS)

clean :
	rm btrfrsc *.o 

