# make build script.
#
# usage:
# 
#    make clean: clean compiled file
#    make cleanAndCompile: clean compiled file and compile the project
#    make compile: compile the project
#    make compileAndRun: compile the project and run the compiled file
#    make run: run the compiled file
#    make all: clean compiled file, compile the project and run the compiled file
#
# author: Manoel NogueiraMelo Filho

UNAME := $(shell uname)
COMPILEDFILE := 8PuzzleSolver

ifeq ($(UNAME), Linux)

	EXE := $(COMPILEDFILE)
	RUN := ./$(COMPILEDFILE)
	RM := rm -f
	NUL := /dev/null

	

else

	EXE := $(COMPILEDFILE).exe
	RUN := $(COMPILEDFILE)
	RM := del /Q
	NUL := nul

endif

CFLAGS := -O1 -Wall -Wextra -Wno-unused-parameter -pedantic-errors -std=c17 -Wno-missing-braces $(shell pkg-config --cflags gtk4) 
LFLAGS := $(shell pkg-config --libs gtk4) -lfontconfig

clean:

	@$(RM) $(EXE) 2>$(NUL)

compile:
	@gcc *.c -o $(EXE) $(CFLAGS) $(LFLAGS)

run:
	@$(RUN)

cleanAndCompile: clean compile
compileAndRun: compile run
all: clean compile run