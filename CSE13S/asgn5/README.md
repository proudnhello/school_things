DESIGN.pdf contains a detailed description of how this program was designed, and the principles behind it.  
DESIGN.tex is the file DESIGN.pdf is derived from.  
WRITEUP.pdf contains a description of how this program was tested to ensure functionality, along with some notes on that functionality.  
WRITEUP.tex is the file WRITEUP.pdf was derived from.  
Makefile is used to help compile the program.  
randstate.c is a small library used to initialize random number generation, and randstate.h is the corresponding header file.  
numtheroy.c is a library used to calculate some number theoretic functions used in RSA encryption. numtheory.h is the corresponding header file. All files wanting to use it must include stdio.h before including numtheroy.h  
rsa.c is a library used to create, output, and read RSA encryption keys. It can also encrypt and decrypt files. rsa.h is the corresponding header file. All files wanting to use it must include stdio.h before including rsa.h  
keygen generates a public / private key pair, placing the keys into the public and private key files as specified below. The keys have a modulus (n) whose length is specified in the program options. It accepts the following command line arguments.  
-s [seed]: Use [seed] as the random number seed. Default: time()  
-b [bits]: Public modulus n must have at least [bits] bits. Default: 1024, range [50, 4092]  
-i [iters]: Run [iters] Miller-Rabin iterations for primality testing. Default: 50, range [1-500]  
-n [pbfile]: Public key file is [pbfile]. Default: rsa.pub  
-d [pvfile]: Private key file is [pvfile]. Default: rsa.priv  
-v: Enable verbose output.  
-h: Display program synopsis and usage.  
encrypt encrypts an input file using the specified public key file, writing the result to the specified output file.  
-i [infile]: Read input from [infile]. Default: standard input.  
-o [outfile]: Write output to [outfile]. Default: standard output.  
-n [keyfile]: Public key is in [keyfile]. Default: rsa.pub.  
-v: Enable verbose output.  
-h: Display program synopsis and usage.  
If someone decides to change the username inside the public key file to something longer than the maximum length of usernames in linux, encrypt may break.  
./decrypt decrypts an input file using the specified private key file, writing the result to the specified output file.  
-i [infile]: Read input from [infile]. Default: standard input.  
-o [outfile]: Write output to [outfile]. Default: standard output.  
-n [keyfile]: Private key is in [keyfile]. Default: rsa.priv.  
-v: Enable verbose output.  
-h: Display program synopsis and usage.  
To use the program, place all of the above files into the same directory. Then, run make from the command line (if one of the programs is required individually, run make keygen, make encrypt, or make decrypt respectively.)  
From there, run ./keygen with the desired command line arguments. Any message can be encrypted with ./encrypt, provided a public key exists in the specified file, which can then be decrypted with ./decrypt and private key.  
If an passed in file for encrypt, decrypt, or keygen cannot be opened, a simple error message is printed without the help message. This is intentional, as while the provided binaries do print out the entire help message on an invalid file, the asgn PDF says to only include the help message when -h is given, an unrecognized option is given, or when a numerical option is out of range, of which this is neither. I decided to follow the pdf, rather than the binaries in this case.