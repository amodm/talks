BUILD_DIR := ./build
SOURCES := $(wildcard *.c)
TARGETS := $(patsubst %.c, %, $(SOURCES))

CC = clang
VFLAGS ?= -fno-slp-vectorize
WFLAGS ?= -Wall -Wextra -Wpedantic -Wno-gnu-statement-expression -Werror
CFLAGS = -O2 -g $(VFLAGS) $(WFLAGS) -std=c18

all: $(TARGETS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

%: %.c *.h $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

clean:
	rm -rf $(BUILD_DIR)

run-%: %
	./$(BUILD_DIR)/$*
