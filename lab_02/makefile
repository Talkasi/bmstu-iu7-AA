CC := g++

CFLAGS := -Wall -Werror -Wpedantic -Wextra -Wfloat-equal -Wfloat-conversion
SRC_LIBS := -lm -lgtest -lgtest_main 
CHECK_LIBS := -lcheck -lm

OUT_DIR := out
SRC_DIR := ./

$(shell mkdir -p $(OUT_DIR))

SRC_OBJS := $(OUT_DIR)/main.o
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

.PHONY: debug
debug: CFLAGS += -g3
debug: debug.lastbuildstate | app.exe

.PHONY: release
release: CFLAGS += -DNDEBUG -g0 -O3
release: release.lastbuildstate | app.exe

%.lastbuildstate:
	make clean
	touch $@

app.exe: $(SRC_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(SRC_LIBS)

$(OUT_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/tests.hpp $(SRC_DIR)/matrix.hpp $(SRC_DIR)/matrix.o
	$(CC) $(CFLAGS) -c $< -o $@ $(SRC_LIBS)

$(OUT_DIR)/matrix.o: $(SRC_DIR)/matrix.hpp
	$(CC) $(CFLAGS) -c $< -o $@ $(SRC_LIBS)

.PHONY: clean
clean:
	$(RM) $(OUT_DIR)/*

.PHONY: format
format:
	clang-format -i -style=microsoft $(INC_DIR)/*.h $(CHECK_DIR)/*.c $(SRC_DIR)/*.c