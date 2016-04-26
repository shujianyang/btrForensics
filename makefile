P = btrfrsc
CFLAGS = -g -Wall -std=c++11
LDLIBS = -ltsk

OBJS = btrfrsc.o SuperBlock.o Guid.o Utility.o

btrfrsc : $(OBJS)
	g++ -o $(P) $(OBJS) $(CFLAGS) $(LDLIBS)

btrfrsc.o : SuperBlock.h Guid.h
	g++ -c btrfrsc.cpp $(CFLAGS) $(LDLIBS)

SuperBlock.o : SuperBlock.h Guid.h Utility.h
	g++ -c SuperBlock.cpp $(CFLAGS) $(LDLIBS)

Guid.o : Guid.h Utility.h
	g++ -c Guid.cpp $(CFLAGS) $(LDLIBS)

Utility.o: Utility.h
	g++ -c Utility.cpp $(CFLAGS) $(LDLIBS)

.PHONY : clean
clean :
	rm btrfrsc *.o 

