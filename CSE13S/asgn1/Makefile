CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: collatz 

collatz: collatz.o
	$(CC) -o collatz collatz.o

collatz.o: collatz.c
	$(CC) $(CFLAGS) -c collatz.c
	
clean:
	rm -f collatz *.o 'figure 1.pdf' 'figure 2.pdf' 'figure 3.pdf' 'figure 4.pdf'

format:
	clang-format -i -style=file *.{c,h}
