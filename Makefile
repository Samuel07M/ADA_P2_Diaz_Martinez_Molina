CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

karatsuba: src/main.o src/algoritmos.o src/operaciones.o
	$(CXX) $(CXXFLAGS) -o karatsuba src/main.o src/algoritmos.o src/operaciones.o

src/main.o: src/main.cpp src/algoritmos.h src/operaciones.h
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o src/main.o

src/algoritmos.o: src/algoritmos.cpp src/algoritmos.h src/operaciones.h
	$(CXX) $(CXXFLAGS) -c src/algoritmos.cpp -o src/algoritmos.o

src/operaciones.o: src/operaciones.cpp src/operaciones.h
	$(CXX) $(CXXFLAGS) -c src/operaciones.cpp -o src/operaciones.o

clean:
	rm -f src/*.o karatsuba