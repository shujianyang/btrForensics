P = btrfrsc
CFLAGS = -g -Wall
LDLIBS = -ltsk

btrfrsc : btrfrsc.o SuperBlock.o Guid.o Utility.o
	g++ -o $(P) btrfrsc.o SuperBlock.o Guid.o Utility.o $(CFLAGS) $(LDLIBS)

btrfrsc.o : btrfrsc.cpp SuperBlock.h Guid.h
	g++ -c btrfrsc.cpp $(CFLAGS) $(LDLIBS)

SuperBlock.o : SuperBlock.cpp SuperBlock.h Guid.h Utility.h
	g++ -c SuperBlock.cpp $(CFLAGS) $(LDLIBS)

Guid.o : Guid.cpp Guid.h Utility.h
	g++ -c Guid.cpp $(CFLAGS) $(LDLIBS)

Utility.o: Utility.cpp Utility.h
	g++ -c Utility.cpp $(CFLAGS) $(LDLIBS)

clear :
	rm btrfrsc *.o 

