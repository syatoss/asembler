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


