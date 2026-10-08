# ===== Configuração =====
TARGET  := rle
CC      := gcc
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -O2 -Iinclude
LDLIBS  :=

SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

# No Windows o executável recebe a extensão .exe
ifeq ($(OS),Windows_NT)
    EXE := .exe
endif

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
BIN  := $(BIN_DIR)/$(TARGET)$(EXE)

# ===== Regras =====
.PHONY: all run test clean

all: $(BIN)

$(BIN): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

run: $(BIN)
	./$(BIN)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Compacta e descompacta cada exemplo e confere se o resultado é idêntico
test: $(BIN)
	@for f in exemplos/*.txt; do \
		./$(BIN) -c $$f $(OBJ_DIR)/teste.rle > /dev/null && \
		./$(BIN) -d $(OBJ_DIR)/teste.rle $(OBJ_DIR)/teste.txt > /dev/null && \
		cmp -s $$f $(OBJ_DIR)/teste.txt && echo "[OK]    $$f" || { echo "[FALHA] $$f"; exit 1; }; \
	done
