.PHONY: all clean spotless format

EXECBIN  = banhammer
CC       = clang
CFLAGS   = -Wall -Wpedantic -Wextra -Werror -Ofast

SOURCES  = $(wildcard *.c)
OBJECTS  = $(SOURCES:%.c=%.o)

all: $(EXECBIN)

$(EXECBIN): bv.o bf.o city.o node.o ll.o ht.o parser.o banhammer.o
	$(CC) -o $@ $^ $(LIBFLAGS)

clean:
	rm -f *.o

spotless:
	rm -f *.o banhammer

format:
	clang-format -i -style=file banhammer.c bf.c bv.c ht.c ll.c node.c parser.c

test: bv.o test.o bf.o city.o node.o ll.o ht.o parser.o
	$(CC) -o $@ $^ $(LIBFLAGS)