# Moore Macauley
# mmacaule
# pa8

#------------------------------------------------------------------------------
#  Makefile for CSE 101 Programming Assignment 8
#
#  make                     makes Order
#  make DictionaryTest      makes DictionaryTest
#  make clean               removes all binaries
#  make OrderCheck          runs Order under valgrind on in5 junk5
#  make DictionaryTest      runs DictionaryTest under valgrind
#------------------------------------------------------------------------------

MAIN           = Order
MAIN2          = WordFrequency
ADT            = Dictionary
SOURCE         = $(MAIN).cpp
SOURCE2         = $(MAIN2).cpp
OBJECT         = $(MAIN).o
OBJECT2         = $(MAIN2).o
ADT_TEST       = $(ADT)Test
ADT_SOURCE     = $(ADT).cpp
ADT_OBJECT     = $(ADT).o
ADT_HEADER     = $(ADT).h
COMPILE        = g++ -std=c++17 -Wall -c
LINK           = g++ -std=c++17 -Wall -o
REMOVE         = rm -f
MEMCHECK       = valgrind --leak-check=full

$(MAIN): $(OBJECT) $(ADT_OBJECT)
	$(LINK) $(MAIN) $(OBJECT) $(ADT_OBJECT)
	
$(MAIN2): $(OBJECT2) $(ADT_OBJECT)
	$(LINK) $(MAIN2) $(OBJECT2) $(ADT_OBJECT)

$(ADT_TEST): $(ADT_TEST).o $(ADT_OBJECT)
	$(LINK) $(ADT_TEST) $(ADT_TEST).o $(ADT_OBJECT)

$(OBJECT): $(SOURCE) $(ADT_HEADER)
	$(COMPILE) $(SOURCE)

$(ADT_TEST).o: $(ADT_TEST).cpp $(ADT_HEADER)
	$(COMPILE) $(ADT_TEST).cpp

$(ADT_OBJECT): $(ADT_SOURCE) $(ADT_HEADER)
	$(COMPILE) $(ADT_SOURCE)

clean:
	$(REMOVE) $(MAIN2) $(OBJECT2) $(MAIN) $(ADT_TEST) $(OBJECT) $(ADT_TEST).o $(ADT_OBJECT)

$(MAIN)Check: $(MAIN)
	$(MEMCHECK) $(MAIN) in5 junk5

$(MAIN2)Check: $(MAIN2)
	$(MEMCHECK) $(MAIN2) in5 junk5

$(ADT)Check: $(ADT_TEST)
	$(MEMCHECK) $(ADT_TEST)
