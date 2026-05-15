CXX = g++
TARGET = build/main

$(TARGET): main.cpp doubleLinkedList.h
	mkdir -p build
	$(CXX) -o $(TARGET) main.cpp

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf build
