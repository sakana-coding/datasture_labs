CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Wpedantic -Iinclude
LDFLAGS :=

BUILD_DIR := build_make
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin

COMMON_SRCS := src/Queens.cpp src/InputUtils.cpp src/SolverTypes.cpp
PLAIN_SRCS := src/PlainSolver.cpp src/main_plain.cpp
MEMO_SRCS := src/MemoSolver.cpp src/main_memo.cpp

COMMON_OBJS := $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(COMMON_SRCS))
PLAIN_OBJS := $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(PLAIN_SRCS))
MEMO_OBJS := $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(MEMO_SRCS))

RUN_MODE_GOALS := $(filter plain memo,$(MAKECMDGOALS))
RUN_BIN :=

ifeq ($(words $(RUN_MODE_GOALS)),1)
  ifeq ($(RUN_MODE_GOALS),plain)
    RUN_BIN := $(BIN_DIR)/queens_plain
  endif
  ifeq ($(RUN_MODE_GOALS),memo)
    RUN_BIN := $(BIN_DIR)/queens_memo
  endif
endif

.PHONY: all plain memo run clean

all: plain memo

plain: $(BIN_DIR)/queens_plain
	@:

memo: $(BIN_DIR)/queens_memo
	@:

run: $(RUN_BIN)
	@if [ -z "$(RUN_BIN)" ]; then \
		echo "Usage: make run plain  或  make run memo"; \
		exit 1; \
	fi
	@./$(RUN_BIN)

$(BIN_DIR)/queens_plain: $(COMMON_OBJS) $(PLAIN_OBJS) | $(BIN_DIR)
	@$(CXX) $(COMMON_OBJS) $(PLAIN_OBJS) -o $@ $(LDFLAGS)

$(BIN_DIR)/queens_memo: $(COMMON_OBJS) $(MEMO_OBJS) | $(BIN_DIR)
	@$(CXX) $(COMMON_OBJS) $(MEMO_OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

clean:
	@rm -rf build build_make
