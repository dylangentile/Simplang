CC=clang++
CFLAGS= -I../include -std=c++11 -Wall -Wextra -Weffc++ -pedantic -g
RM=rm


all: slc

slc: main.o analyze.o parser.o lexer.o statement.o typing.o error.o format.o os.o 
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<


clean:
	$(RM) *.o
	$(RM) -rf *.dSYM
	$(RM) slc
.PHONY: clean


