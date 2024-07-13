CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
BINS = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%, $(SRCS))

.PHONY: all
all: $(BINS) $(OBJS) copy_main

$(BIN_DIR)/%: $(OBJ_DIR)/%.o
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

copy_main:
	cp $(BIN_DIR)/main ./escalona

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) escalona
