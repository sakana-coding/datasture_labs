CMAKE ?= cmake
BUILD_DIR ?= build
TARGET ?= airport

BINARY := $(BUILD_DIR)/bin/$(TARGET)

.PHONY: all configure debug release run lldb clean rebuild

all: debug

configure:
	$(CMAKE) -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug

debug: configure
	$(CMAKE) --build $(BUILD_DIR)

release:
	$(CMAKE) -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release
	$(CMAKE) --build $(BUILD_DIR)

run: debug
	./$(BINARY)

lldb: debug
	lldb ./$(BINARY)

clean:
	$(CMAKE) --build $(BUILD_DIR) --target clean

rebuild: clean debug
