P = btrfrsc
CFLAGS = -g -Wall
LDLIBS = -ltsk

btrfrsc : btrfrsc.o guid.o
	g++ -o $(P) btrfrsc.o guid.o $(CFLAGS) $(LDLIBS)

btrfrsc.o : btrfrsc.cpp guid.h
	g++ -c btrfrsc.cpp $(CFLAGS) $(LDLIBS)

guid.o : guid.cpp guid.h
	g++ -c guid.cpp $(CFLAGS) $(LDLIBS)

clean :
	rm btrfrsc *.o 

