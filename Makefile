main: main.cpp VideoAnalyzer.h VideoAnalyzer.cpp
	g++ -std=c++11 -o main.o main.cpp `pkg-config opencv --cflags --libs`
clean:
	rm *.o
