DESIGN.pdf offers a detailed breakdown of how this program was designed, and it is derived through pdflatex via DESIGN.tex.  
WRITEUP.pdf gives a description of the conclusions reached through the use of this program about the effectiveness of various sorting algorithms.  
To run this program, be sure to include:  
bubble.c and bubble.h  
heap.c and heap.h  
mtrand.c and mtrand.h  
quick.c and quick.h  
set.c and set.h  
shell.c and shell.h  
stats.c and stats.h  
and finally, sorting.c and Makefile in the same directory.   
To compile the program, run make in the command line while in the directory the above code is in.  
To use the program, run ./sorting [-Hasbhq] [-n length] [-p elements] [-r seed]  
-H will display program help and usage.  
-a will enable all sorts.  
-b will enable Bubble Sort.  
-h will enable Heap Sort.  
-q enable Quick Sort.  
-s enable Shell Sort.  
-n length will specify the number of array elements (default: 100, range: 0 <= length <= 250000000).  
-p elements will specify the number of elements to print (default: 100, range 0 <= elements).  
-r seed will specify random seed (default: 13371453, range: 0 <= seed).  
The program will generate a random array of the specified length using the provided seen, and then sort that array with the specified sorting algorithms.  
It will then print out the name of the algorithm, the number of array accesses made, the number of times array elements were compared, before listing the specified number of array elements.  
To generate the graphs seen in WRITEUP.tex, you should use the scripts graphing.sh and graphing_no_bubblesort.sh.  
Some code will have to be commented out and uncommented in sorting.c, see comments in code there for more.