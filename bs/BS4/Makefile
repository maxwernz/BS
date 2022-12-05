sort : sort.o sort_merge.o
		g++ sort.o sort_merge.o -pthread -o sort

sort.o: sort.cpp sort_merge.h
		g++ -c -pthread sort.cpp

sort_merge.o : sort_merge.cpp sort_merge.h
			g++ -c sort_merge.cpp