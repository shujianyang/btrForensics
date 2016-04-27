P = btrfrsc
CFLAGS = -g -Wall -std=c++11
LDLIBS = -ltsk

OBJS = btrfrsc.o SuperBlock.o Uuid.o DevItem.o Utility.o

btrfrsc : $(OBJS)
	g++ -o $(P) $(OBJS) $(CFLAGS) $(LDLIBS)

btrfrsc.o : SuperBlock.h Uuid.h
	g++ -c btrfrsc.cpp $(CFLAGS) $(LDLIBS)

SuperBlock.o : SuperBlock.h Uuid.h DevItem.h Utility.h
	g++ -c SuperBlock.cpp $(CFLAGS) $(LDLIBS)

Uuid.o : Uuid.h Utility.h
	g++ -c Uuid.cpp $(CFLAGS) $(LDLIBS)

DevItem.o : Uuid.h Utility.h
	g++ -c DevItem.cpp $(CFLAGS) $(LDLIBS)

Utility.o: Utility.h
	g++ -c Utility.cpp $(CFLAGS) $(LDLIBS)

.PHONY : clean
clean :
	rm btrfrsc *.o 

