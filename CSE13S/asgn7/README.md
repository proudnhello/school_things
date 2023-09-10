In order to compile this file, be sure to include the following files in the same directory:  
code.c and code.h, for managing the character codes created by the huffman tree.  
node.c and node.h, which implements the nodes of the huffman tree.  
pq.c and pq.h, which creates a priority queue of nodes based on their frequencies.  
stack.c and stack.h, which creates a stack of nodes.  
io.h and io.c, which deals with reading and writing trees and characters from files.  
huffman.c and huffman.h, which construct the huffman trees.  
encode.c and decode.c, which do the encoding and decoding of files, respectively.  
header.h, which defines the header used in files created with encode.c  
defines.h, which defines some magic numbers used throughout all the above files.
and finally, the Makefile, which is used to automate the compilation of the program.  
 
Also included, but not required for compilation is DESIGN.pdf and DESIGN.tex, which are a document detailing the design of these files and the source file it is derived from, respectively.  
 
To use encode, simply run make encode or make all.  
Encode is a huffman encoder. It compresses a file using the Huffman coding algorithm.  
To use it, run ./encode [-h] [-i infile] [-o outfile], where  
-h prints program usage and help.  
-v prints compression statistics.  
-i infile specifies the input file to compress (default stdin).  
-o outfile specifies the output of compressed data (default stdout).  
 
To use decode, simply run make decode or make all.  
Decode is a huffman decoder. It decompresses a file using the Huffman coding algorithm.  
To use it, run ./decode [-h] [-i infile] [-o outfile], where  
-h prints program usage and help.  
-v prints compression statistics.  
-i infile specifies the input file to decompress (default stdin).  
-o outfile specifies the output of decompressed data (default stdout).  
 
To help me understand Huffman Encoding, I watched this video https://www.youtube.com/watch?v=JsTptu56GM8  
In order to help me test my constructed trees, I created a print tree function heavily based on one provided by Athanasius Kircher#1515 (who I suspect to be Dr. Long, though I only have the testimony of a single alumni to back me up) on discord. No other code or pseudocode was provided by him. The function only prints out a huffman tree in a readable format, and while it is not used at all in any of my final code, you can find it commented out above main in encode and decode.  

Write bits cannot track how big the buffer passed into it is. As such, if nbytes is larger than the size of the buffer, the result is unpredictable.  
Files to be encoded are stored in memory while they are being encoded. As such, very large files on systems with very small amounts of memory will result in issues.