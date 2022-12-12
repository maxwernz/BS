all: client.cpp server.cpp
	g++ client.cpp -o client -lrt
	g++ server.cpp -o server -pthread -lrt

clean:
	rm client server