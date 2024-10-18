CC := g++

CFLAGS := -Wall -Werror -Wpedantic -Wextra -Wfloat-equal -Wfloat-conversion
SRC_LIBS := -lm -lgtest -lgtest_main 
CHECK_LIBS := -lcheck -lm

OUT_DIR := out
SRC_DIR := ./

$(shell mkdir -p $(OUT_DIR))

SRC_OBJS := $(OUT_DIR)/main.o $(OUT_DIR)/matrix.o
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

$(OUT_DIR)/main.o: $(SRC_DIR)/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(SRC_LIBS)

$(OUT_DIR)/matrix.o: $(SRC_DIR)/matrix.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(SRC_LIBS)

app-cli-debug: app.exe
	mkdir -p ./ready
	cp app.exe ready/app-cli-debug

.PHONY: clean
clean:
	$(RM) $(OUT_DIR)/* *.out *.o *.exe

.PHONY: format
format:
	clang-format -i -style=microsoft $(SRC_DIR)/*.hpp $(SRC_DIR)/*.cpp ./docs/inc/lst/*