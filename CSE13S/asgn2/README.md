To construct the program, simply navigate to the directory it is contained in, then run make.  
To run the program, run ./mathlib-test.  
With no arguments, the program will do nothing.  
Run with -a, the program will run all of the tests mentioned below.  
Given -s, the program will output a comparison of my_sin(), implemented in mathlib.c, against the sin() function in math.h, in the range [0,2pi], with a step of 0.05pi  
Given -c, the program will do the same thing, except with my_cos() and cos().  
Given -S, the program will compare my_arcsin() and asin() in the range [-1,1), with a step of 0.05.  
Given -C, the program will do the same thing, but with my_arccos() and acos().  
Given -T, the program will compare my_arctan() aginst atan(), in the range [1, 10), with a step of 0.05.  
Given -l, the program will do the same thing, but with my_log() and log().