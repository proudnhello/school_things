#include <math.h>
#include "mathlib.h"
#define EPSILON 1e-10

double square_root (double x) {
	double m, l = 0.0;
	double h = (x < 1) ? 1 : x;
	double epsilon = 1e-10;
	double range;
	do {
  		m = (l + h) / 2.0;
    	if (m * m < x) {
    	l = m;
    	} else {
    	h = m;
    	}
    	range = (l > h) ? l - h : h - l;
	} while (range > epsilon);
  return m;
}

double Exp(double x) {
	double t = 1.0;
	double y = 1.0;
	for (double k = 1.0; t > EPSILON; k += 1.0) {
		t *= x / k;
		y += t;
	}
	return y;
}

double my_log(double x){
	//We need this to run once, so we set exit to something that we know is larger than EPSILON
	double current = 1.0, exit = 1.0;
	while (exit > EPSILON){
		double numerator = x - (Exp(current));
		double denominator = Exp(current);
		current += (numerator/denominator);
		exit = (Exp(current)) - x;
		if (exit < 0){
			exit *= -1;
		}
	}
	return current;
}

double my_sin(double x){
	//As sin is odd, -sin(x) = sin(-x), so we find sin(x), then return the negitive of that
	int neg = 0;
	if (x < 0){
		x *= -1;
		neg = 1;
	}
	double previous = x, total = x, n = 0.0, current;
	do{
		n += 1;
		// Calculates the next term of the sequence
		double fraction1 = x / (2.0*n);
		double fraction2 = x / ((2.0*n) + 1.0);
		current = previous * fraction2 * fraction1;
		// Simply casting the double n to an int in variabe N
		int N = n;
		if (N % 2 == 1){
			//If n is odd, then the -1^n in front = -1, so we should subtract the current term from the total
			total -= current;
		}else{
			//Otherwise, -1^n = 1, so we should add
			total += current;
		}
		previous = current;
	}while (current > EPSILON);
	//Finishing negative work
	if (neg){
		return (total * -1);
	}else{
		return total;
	}

}

double my_cos(double x){
	//As cos is even, cos(x) = cos(-x), so if given -x, we simply find and return cos(x)
	if (x < 0){
		x *= -1;
	}
	double previous = 1, total = 1, n = 0.0, current;
	do{
		n += 1;
		// Calculates the next term of the sequence
		double fraction1 = x / (2.0*n);
		double fraction2 = x / ((2.0*n) - 1.0);
		current = previous * fraction2 * fraction1;
		// Simply casting the double n to an int in variabe N
		int N = n;
		if (N % 2 == 1){
			//If n is odd, then the -1^n in front = -1, so we should subtract the current term from the total
			total -= current;
		}else{
			//Otherwise, -1^n = 1, so we should add
			total += current;
		}
		previous = current;
	}while (current > EPSILON);
	return total;
}

double my_arcsin(double x){
	double current;
	double previous = x, exit = 1;
		while(exit > EPSILON){
			double sin_val = my_sin(previous);
			double cos_val = my_cos(previous);
			current = previous - ((sin_val - x) / cos_val);
			exit = current - previous;
			if (exit < 0){
				exit *= -1;
			}
			previous = current;
		}
	return current;
	
}

double my_arccos(double x){
	double temp = my_arcsin(x);
	return (M_PI/2) - temp;
}

double my_arctan(double x){
	double temp = square_root((x*x + 1));
	return my_arcsin((x/temp));
}


