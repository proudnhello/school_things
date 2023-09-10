.PHONY: all clean spotless format

CC       = clang
CFLAGS   = -Wall -Wpedantic -Wextra -gdwarf-4 -Werror

SOURCES  = $(wildcard *.c)
OBJECTS  = $(SOURCES:%.c=%.o)

all: encode decode

encode: encode.o node.o stack.o pq.o code.o io.o huffman.o
	$(CC) -o $@ $^ $(LIBFLAGS)

decode: decode.o node.o stack.o pq.o code.o io.o huffman.o
	$(CC) -o $@ $^ $(LIBFLAGS)

clean:
	rm -f *.o

spotless:
	rm -f *.o encode decode

format:
	clang-format -i -style=file *.c
