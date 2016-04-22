P = btrfrsc
CFLAGS = -g -Wall
LDLIBS = -ltsk

btrfrsc : btrfrsc.o SuperBlock.o Guid.o
	g++ -o $(P) btrfrsc.o SuperBlock.o Guid.o $(CFLAGS) $(LDLIBS)

btrfrsc.o : btrfrsc.cpp SuperBlock.h Guid.h
	g++ -c btrfrsc.cpp $(CFLAGS) $(LDLIBS)

SuperBlock.o : SuperBlock.cpp SuperBlock.h Guid.h
	g++ -c SuperBlock.cpp $(CFLAGS) $(LDLIBS)

Guid.o : Guid.cpp Guid.h Utility.h
	g++ -c Guid.cpp $(CFLAGS) $(LDLIBS)

clean :
	rm btrfrsc *.o 

