CXX ?= g++
CXXFLAGS ?= -std=c++11 -Wall -Wextra -pedantic
GLFW_CFLAGS := $(shell pkg-config --cflags glfw3 2>/dev/null)
GLFW_LIBS := $(shell pkg-config --libs glfw3 2>/dev/null)
GUI_LIBS := $(GLFW_LIBS) -framework OpenGL -framework ImageIO -framework CoreGraphics -framework CoreFoundation

# 目录配置
BUILD_DIR := build
INCLUDE_DIR := include
SRC_DIR := src

# 可执行文件和源文件
TARGET := $(BUILD_DIR)/tic_tac_toe
GUI_TARGET := $(BUILD_DIR)/tic_tac_toe_gui
SRCS := $(SRC_DIR)/main.cpp \
        $(SRC_DIR)/TicTacToeBoard.cpp \
        $(SRC_DIR)/ComputerPlayer.cpp \
        $(SRC_DIR)/TicTacToeGame.cpp
GUI_SRCS := $(SRC_DIR)/main2.cpp \
            $(SRC_DIR)/TicTacToeBoard.cpp \
            $(SRC_DIR)/ComputerPlayer.cpp

# 把 src/xxx.cpp -> build/src/xxx.o
OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))
GUI_OBJS := $(patsubst %.cpp,$(BUILD_DIR)/gui/%.o,$(GUI_SRCS))
DEPS := $(OBJS:.o=.d) $(GUI_OBJS:.o=.d)

.PHONY: all run gui run-gui clean

# 默认目标：只编译，所有产物放到 build 目录
all: $(TARGET)

# make run：先编译，再运行 build 里的可执行文件
run: $(TARGET)
	./$(TARGET)

gui: $(GUI_TARGET)

run-gui: $(GUI_TARGET)
	./$(GUI_TARGET)

# 链接可执行文件
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(GUI_TARGET): $(GUI_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(GUI_LIBS)

# 编译每个 .cpp；-Iinclude 用来找到头文件
# -MMD -MP 会自动生成依赖文件，修改头文件后也能正确重新编译。
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -MMD -MP -c $< -o $@

$(BUILD_DIR)/gui/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(GLFW_CFLAGS) -I$(INCLUDE_DIR) -MMD -MP -c $< -o $@

# make clean：清除 build 文件夹里的内容，但保留 build 文件夹本身
clean:
	@mkdir -p $(BUILD_DIR)
	rm -rf $(BUILD_DIR)/*

-include $(DEPS)
