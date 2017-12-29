GCC_FLAGS = g++ -std=c++11 -Wextra -Wall 

default: GenericMatrixDriver
	./GenericMatrixDriver

GenericMatrixDriver: GenericMatrixDriver.o Complex.o Matrix
	$(GCC_FLAGS) GenericMatrixDriver.o Complex.o -o GenericMatrixDriver 

GenericMatrixDriver.o: GenericMatrixDriver.cpp Matrix.hpp Complex.h
	$(GCC_FLAGS) -c GenericMatrixDriver.cpp -o GenericMatrixDriver.o

Matrix: Complex.h Complex.cpp Matrix.hpp
	$(GCC_FLAGS) -c Matrix.hpp
	
Complex.o: Complex.h Complex.cpp
	$(GCC_FLAGS) Complex.cpp -c -o Complex.o 

tar: 
	tar cvf ex3.tar Matrix.hpp Makefile extension.pdf

valgrind:
	valgrind --leak-check=full --show-possibly-lost=yes --show-reachable=yes --undef-value-errors=yes ./GenericMatrixDriver

clean:
	rm *.o GenericMatrixDriver Matrix.hpp.gch
