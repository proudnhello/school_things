To use this program, ensure that you have the following files inside the same directory:  
banhammer.c, which contains the main function.  
city.c and city.h, which contain a hash function produced by Alexander Nusov.  
ht.c and ht.h, the implementation and ADT for my hash table.  
ll.c and ll.h, the implementation and ADT for my linked list used in my hash table.  
node.c and node.h, the implementation and ADT for the nodes used in my linked list.  
bf.c and bf.h, the implementation and ADT for my bit vector, used by my bloom filter.  
bv.c and bv.h, the implementation and ADT for my bloom filter.  
parser.c and parser.h, the implementation and ADT for my text parser.  
The provided Makefile  
 
Also included but not required for compiling are:  
DESIGN.pdf and DESIGN.tex, a document detailing the design of the above file, and the source file it was generated from.  
WRITEUP.pdf and WRITEUP.tex, a document going into the efficiency of some aspects of the above functions and the source file it was generated from.  
graphmaker1-4.sh, bash scripts that use gnuplot to create the graphs seen in WRITEUP.pdf  
 
To compile the program, simply run make or make all from the command line.  
To remove the generated binaries, run make clean, and to remove the binaries and the executable, run make spotless.  
make format can also be run to format some of the source code.  
 
To run the program, run ./banhammer in the command line.  
./banhammer will read in words from stdin, identify any badspeak or old speak and output an appropriate punishment message. The badspeak and oldspeak (with the newspeak translation) that caused the punishment will be printed after the message. If statistics are enabled punishment messages are suppressed and only statistics will be printed.  
-t [ht_size]: Hash table size set to [ht_size]. (default: 10000, minimum of 1)  
-f [bf_size]: Bloom filter size set to [bf_size]. (default 2^19, minimum of 1)  
-s : Enables the printing of statistics.  
-m: Enables move-to-front rule.  
-h: Display program synopsis and usage.  
 
I was recommended by Athanasius Kircher#1515 (who I think is Professor Long?) on discord to use gprof to find where my code was spending most of its time.  
hot dog#9075 (also on discord) suggested testing my code when there was no newline before the end of a file, which allowed me to find that bug. The fix for it was entirely my idea.  
I used clang.llvm.org to create a clang format file to my preferences.  

If a line longer than 1000 characters is passed into parser, the result is unpredictable.