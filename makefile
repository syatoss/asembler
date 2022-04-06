# CC = gcc
# CFLAGS = -ansi -Wall -pedantic
# MAIN = main
# BIN = bin
# SRC = src
# PRE_PROC = pre_proc
# HEADERS = headers
# OBJECTS := $(BIN)/$(wildcard *.o)


# compile-vebose: $(BIN)/$(MAIN)
# 	@echo "Compiled sucsessfully path to bin file: $(BIN)/$(MAIN)"
# 	@echo "Try \"make run\" commad to execute the program"

# $(BIN)/$(MAIN): $(BIN) $(BIN)/$(MAIN).o
# 	@echo "making file $(BIN)/$(MAIN)"
# 	@$(CC) -g  $(OBJECTS) -o $(BIN)/$(MAIN)

# $(BIN)/$(MAIN).o: $(BIN) $(SRC)/$(MAIN).c
# 	@echo "making file $(BIN)/$(MAIN).o"
# 	@$(CC) -c $(CFLAGS) $(SRC)/$(MAIN).c -o $(BIN)/$(MAIN).o

# run: $(BIN) $(BIN)/$(MAIN)
# 	@echo "Runnig program $(MAIN):"
# 	@$(BIN)/$(MAIN)

# clean:
# 	@echo "Deleting all object files"
# 	@rm -rf $(BIN)/*

# $(BIN):
# 	@mkdir $(BIN)

CC=gcc
CFLAGS=-ansi -Wall -pedantic
BIN = bin
SRC = src
MAIN_NAME = main
MAIN_SRC = $(SRC)/$(MAIN_NAME)
MAIN_BIN = $(BIN)/$(MAIN_NAME)
HEADERS_DIR = $(SRC)/headers
COMPONENTS_DIR = $(SRC)/components
COMPONENTS_SOURCE = $(patsubst %/, %,$(sort $(dir $(wildcard $(COMPONENTS_DIR)/*/))))
COMPONENTS_HEADERS = $(patsubst %/, %,$(sort $(dir $(wildcard $(HEADERS_DIR)/*/))))
COMPONENTS_NAMES = $(patsubst $(COMPONENTS_DIR)/%,%, $(COMPONENTS_SOURCE))
OBJS = $(foreach PATH, $(patsubst %.c, $(BIN)/%.o, $(wildcard $(COMPONENTS_DIR)/*/*.c)), $(BIN)/$(lastword $(subst /, ,$(PATH))))
GLOBAL_HEADERS = $(SRC)/constansts.h $(HEADERS_DIR)/master_header.h

$(BIN)/%.o: $(COMPONENTS_DIR)/%.c $(HEADERS_DIR)/%.h $(GLOBAL_HEADERS) $(BIN)/$(COMPONENTS_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(MAIN_BIN): $(OBJS) $(MAIN_BIN).o
	$(CC) -o $@ $^ $(CFLAGS)


$(MAIN_BIN).o: $(MAIN_SRC).c $(MAIN_SRC).h
	$(CC) -o $@ $< $(CFLAGS)

list-comp-source:
	@echo $(COMPONENTS_SOURCE)

list-headers:
	@echo $(COMPONENTS_HEADERS)

list-comp-names:
	@echo $(COMPONENTS_NAMES)

list-objs:
	@echo $(OBJS)

list-all-headers:
	@echo $(ALL_HEADERS)

.PHONY: clean

$(BIN):
	mkdir $(BIN)

clean:
	rm -rf $(BIN)/*
