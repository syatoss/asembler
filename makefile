CC = gcc
CFLAGS = -ansi -g -Wall -pedantic -lm
MAIN = assembler
BIN = bin
SRC = src
HEDEARS = headers
OBJS = $(patsubst $(SRC)/%.c,$(BIN)/%.o,$(wildcard $(SRC)/*.c))
TESTDIR = tests
EXE = ./$(MAIN)


#auto rule to generate objects files
$(BIN)/%.o: $(SRC)/%.c $(wildcard $(HEADERS)/*.h)
	$(CC) -c -o $@ $< $(CFLAGS)

default: $(BIN) $(MAIN)
	@echo ""
	@echo ""
	@echo "Project binary is found in the current directory under the name '$(MAIN)'"
	@echo "Sample tests cases can be found in '$(TESTDIR)' directory"
	@echo ""
	@echo "Use the following commands to run the project:"
	@echo "    - 'make clean' to delete all previous binaries" 
	@echo "    - 'make test' to run all the test cases and see" 
	@echo "      their output files in their respective directories" 
	@echo "      under the $(TESTDIR) directory" 

compile: $(BIN) $(MAIN)

$(MAIN): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

run: compile
	$(EXE)


$(BIN):
	@mkdir bin

.PHONY: clean run

objs-print:
	@echo $(OBJS)

clean:
	rm -rf bin
	rm $(MAIN)

test: compile
	$(EXE) $(TESTDIR)/shouldCompileSuccessfuly/shouldCompileSuccessfully $(TESTDIR)/shouldErrorData/shouldErrorData $(TESTDIR)/shouldErrorEntry/shouldErrorEntry $(TESTDIR)/shouldErrorExtern/shouldErrorExtern $(TESTDIR)/shouldErrorInvalidOperation/shouldErrorInvalidOperation $(TESTDIR)/shouldErrorJmp/shouldErrorJmp $(TESTDIR)/shouldErrorString/shouldErrorString $(TESTDIR)/shouldErrorStruct/shouldErrorStruct $(TESTDIR)/shouldErrorUndefinedLable/shouldErrorUndefinedLable $(TESTDIR)/shouldNotFindFile/shouldNotFindFile


