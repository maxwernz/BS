semtest: semtest.o
	g++ semtest.o -pthread -lsem -L ~/nethome/BS/export -o semtest

semtest.o: semtest.cpp
	g++ -c semtest.cpp -pthread