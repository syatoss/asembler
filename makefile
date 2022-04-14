CC = gcc
CFLAGS = -ansi -Wall -pedantic
MAIN = main
BIN = bin
SRC = src
HEDEARS = headers
OBJS = $(patsubst $(SRC)/%.c,$(BIN)/%.o,$(wildcard $(SRC)/*.c))
TESTDIR = tests
EXE = ./$(BIN)/$(MAIN)

#auto rule to generate objects files
$(BIN)/%.o: $(SRC)/%.c $(wildcard $(HEADERS)/*.h)
	$(CC) -c -o $@ $< $(CFLAGS)

default: $(BIN) $(BIN)/$(MAIN)

$(BIN)/$(MAIN): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

run: default
	$(EXE)


$(BIN):
	@mkdir bin

.PHONY: clean run

objs-print:
	@echo $(OBJS)

clean:
	rm -rf bin

test-argv: default
	$(EXE) $(TESTDIR)/main/argv-read




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

# CC=gcc
# CFLAGS=-ansi -Wall -pedantic
# BIN = bin
# SRC = src
# MAIN_NAME = main
# MAIN_SRC = $(SRC)/$(MAIN_NAME)
# MAIN_BIN = $(BIN)/$(MAIN_NAME)
# HEADERS_DIR = $(SRC)/headers
# COMPONENTS_DIR = $(SRC)/components
# COMPONENTS_SOURCE = $(patsubst %/, %,$(sort $(dir $(wildcard $(COMPONENTS_DIR)/*/))))
# COMPONENTS_HEADERS = $(patsubst %/, %,$(sort $(dir $(wildcard $(HEADERS_DIR)/*/))))
# COMPONENTS_NAMES = $(patsubst $(COMPONENTS_DIR)/%,%, $(COMPONENTS_SOURCE))
# OBJS = $(foreach PATH, $(patsubst %.c, $(BIN)/%.o, $(wildcard $(COMPONENTS_DIR)/*/*.c)), $(BIN)/$(lastword $(subst /, ,$(PATH))))
# GLOBAL_HEADERS = $(SRC)/constansts.h $(HEADERS_DIR)/master_header.h

# $(BIN)/%.o: $(COMPONENTS_DIR)/%.c $(HEADERS_DIR)/%.h $(GLOBAL_HEADERS) $(BIN)/$(COMPONENTS_DIR)
# 	$(CC) -c -o $@ $< $(CFLAGS)

# $(MAIN_BIN): $(OBJS) $(MAIN_BIN).o
# 	$(CC) -o $@ $^ $(CFLAGS)


# $(MAIN_BIN).o: $(MAIN_SRC).c $(MAIN_SRC).h
# 	$(CC) -o $@ $< $(CFLAGS)

# list-comp-source:
# 	@echo $(COMPONENTS_SOURCE)

# list-headers:
# 	@echo $(COMPONENTS_HEADERS)

# list-comp-names:
# 	@echo $(COMPONENTS_NAMES)

# list-objs:
# 	@echo $(OBJS)

# list-all-headers:
# 	@echo $(ALL_HEADERS)

# .PHONY: clean

# $(BIN):
# 	mkdir $(BIN)

# clean:
# 	rm -rf $(BIN)/*
# 	PROJECT_NAME := example-project
# PROJECT_NAME := main
# CC        := gcc
# SRCDIR    := src
# HEADERDIR := include
# BUILDDIR  := build
# BINDIR    := bin
# TARGET    := $(BINDIR)/$(PROJECT_NAME)
# SOURCES   := $(shell find $(SRCDIR) -type f -name *.c*)
# HEDEARS   := $(shell find $(HEADERDIR) -type f -name *.h*)
# OBJECTS   := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(addsuffix .o,$(basename $(SOURCES))))
# DEPS      := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(addsuffix .d,$(basename $(SOURCES))))
# CFLAGS    := ansi -Wall pedantic
# LIB       :=
# INC       :=
# all: debug
# debug: CFLAGS += -g
# debug: $(TARGET)
# release: $(TARGET)
# release: CFLAGS += -O3

# GREEN=`tput setaf 2`
# RESET=`tput sgr0`

# define print_green
# 	@echo "$(GREEN)$(1)$(RESET)"
# endef

# all: $(TARGET)

# clean:
# 	rm $(BUILDDIR) $(BINDIR) -rf

# $(TARGET): $(BINDIR) $(BUILDDIR) $(OBJECTS)
# 	$(call print_green,"Linking object files...")
# 	$(CC) $(OBJECTS) -o $(TARGET) $(LIB)
# 	$(call print_green,"$(TARGET) has been created!")

# $(BUILDDIR) :
# 	mkdir $(BUILDDIR)

# $(BINDIR):
# 	mkdir $(BINDIR)

# $(BUILDDIR)/%.o: $(SRCDIR)/%.c*
# 	mkdir -p $(dir $@)
# 	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
# 	$(CC) $(CFLAGS) $(INC) -M $< -MT $@ > $(@:.o=.td)
# 	cp $(@:.o=.td) $(@:.o=.d);
# 	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
# 	-e '/^$$/ d' -e 's/$$/ :/' < $(@:.o=.td) >> $(@:.o=.d);
# 	rm -f $(@:.o=.td)


# -include $(DEPS)

# .PHONY: clean all


