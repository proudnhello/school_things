#Moore Macauley
#mmacule
#pa1

#------------------------------------------------------------------------------
# Makefile for CSE 101 Programming Assignment 1
#
# make                   makes Lex
# make ListClient        makes ListClient
# make test              makes test based on ListTest
# make clean             removes all binaries
# make checkTest         checks listTest for mem errors
# make checkClient       checks ListClient for memory errors
# make checkLex          checks Lex for memory errors
#------------------------------------------------------------------------------

Lex : Lex.o List.o
	gcc -std=c17 -Wall -o Lex Lex.o List.o

Lex.o : Lex.c List.h
	gcc -std=c17 -Wall -c Lex.c

ListClient: ListClient.o List.o
	gcc -std=c17 -Wall -o ListClient ListClient.o List.o

ListClient.o : ListClient.c List.h
	gcc -std=c17 -Wall -c ListClient.c

List.o : List.c List.h
	gcc -std=c17 -Wall -c List.c

ListTest.o : ListTest.c List.h
	gcc -std=c17 -Wall -c ListTest.c

test : ListTest.o List.o
	gcc -std=c17 -Wall -o test ListTest.o List.o

clean :
	rm -f Lex ListClient Lex.o ListClient.o List.o ListTest.o test

checkClient : 
	valgrind --leak-check=full ./ListClient

checkLex :
	valgrind --leak-check=full ./Lex input output

checkTest : 
	valgrind --leak-check=full ./test
