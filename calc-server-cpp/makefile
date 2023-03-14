server: bin/server.o
	clang++ -Wall -std=c++20 server.o tcp.o -o server

bin/server.o: server.cpp lib/tcp.cpp lib/tcp.h
	clang++ -Wall -std=c++20 -c server.cpp lib/tcp.cpp

clean:
	rm -f *.o server
