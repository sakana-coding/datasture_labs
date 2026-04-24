CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -O2
TARGET := polynomial
OBJS := main.o Polynomial.o

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

main.o: main.cpp Polynomial.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Polynomial.o: Polynomial.cpp Polynomial.h
	$(CXX) $(CXXFLAGS) -c Polynomial.cpp

.PHONY: clean run
run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
