DESIGN.pdf is a file detailing the design of the other created files.  
DESIGN.tex is a file used to create DESIGN.pdf with pdflatex.  
Makefile uses the command line argument make to compile the program automatically. make clean can also be used to remove all derived files from the directory.  
dreidel.c supports play-dreidel.c, providing the simulation of the game and dreidel spinning functionality.  
dreidel.h is used to link dreidel.c and play-dreidel.c.  
mtrand.c is code provided by Takuji Nishimura and Makoto Matsumoto, and uses a Mersenne Twister pseudo random number generator to simulate dreidel spins.  
mtrand.h links mtrand.c and dreidel.c.  
play-dreidel.c simulates a game of dreidel being played, by default with 4 players, each player having 3 coins at the start, with the seed 613. It then outputs the name of the winning player, the number of players, the starting number of coins, the number of rounds the game took, and the seed that was used.  
-p [x] tells the program that the game should have x players. x must be between 2 and 8, inclusive.  
-c [x] tells the program that all players should start with x coins. x must be between 1 and 20, inclusive.  
-s [x] tells the program that the seed should be x. Seeds can only be between 0 and 10^10, inclusive.  
-v will cause the program to print a message every time a player is eliminated, containing the player that was eliminated.  
WRITEUP.pdf is a file that responds to questions that I used this program to answer.  
WRITEUP.tex is a file used to create WRITEUP.pdf with pdflatex.  
graph1.sh is a bash script used to generate some of the graphs seen in WRITEUP.pdf. To use it, the user must comment out the section marked for use with default functionality, and restore the section marked for use with graph1.sh and graph2.sh in play-dreidel.c. Lines 67 and 69 + 70, respectively.  
I would have made the above and below steps unnecessary with a command line argument used with play-dreidel.c, but I was concerned that doing so would be deemed unacceptable.  
graph2.sh is a bash script used to generate some of the graphs seen in WRITEUP.pdf. To use it, the user must comment out the section marked for use with default functionality, and restore the section marked for use with graph1.sh and graph2.sh in play-dreidel.c. Lines 67 and 69 + 70, respectively.  
graph3.sh is a bash script used to generate some of the graphs seen in WRITEUP.pdf. To use it, the user must comment out the section marked for use with default functionality, and restore the section marked for use with graph3.sh in play-dreidel.c. Lines 67 and 72, respectively.