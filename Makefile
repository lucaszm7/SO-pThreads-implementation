CXX=g++
CXXFLAGS= -g -Wall -lpthread
LFLAGS= -lpthread -export-dynamic


all: $(OBJS)
	$(CXX) -o exemplo exemplo.o $(LFLAGS)

	
run: exemplo
	./exemplo

clean:
	rm -rf *.o

exemplo.o: exemplo.cpp
	$(CXX) -c $(CXXFLAGS) exemplo.cpp -o exemplo.o
