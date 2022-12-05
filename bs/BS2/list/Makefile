CXXFLAGS=-g

listtest: listtest.o linkedlist.o
	gcc listtest.o linkedlist.o -o listtest

listtest.o: listtest.c linkedlist.h
	gcc $(CXXFLAGS)  -c listtest.c

linkedlist.o: linkedlist.c linkedlist.h
	gcc $(CXXFLAGS)  -c linkedlist.c

clean:
	-rm linkedlist.o listtest.o listtest
