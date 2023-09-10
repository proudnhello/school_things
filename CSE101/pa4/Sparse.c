//Moore Macauley
//mmacule
//pa4

#include "Matrix.h"
#include "List.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_LENGTH 300

// Used to read in A and B, simply reads in a matrix in the provided format
void readInMatrix(Matrix M, int lineNum, FILE* in, const char* fileName){
    char line[MAX_LENGTH];
    int w = 1;
    int *input1 = &w;
    int x = 1;
    int *input2 = &x;
    double y = 1;
    double *input3 = &y;
    int z = 1;
    int *throwaway = &z;
    // For every line that the maxtrix is made up by, read it in and change the corrisponding entry
    for(int i = 0; i < lineNum; i++){
        fgets(line, MAX_LENGTH, in);
        if(3 != sscanf(line, "%d %d %lf %d\n", input1, input2, input3, throwaway)){
            fprintf(stderr,"%s is in incorrect format\n", fileName);
            exit(1);
        }
        changeEntry(M, *input1, *input2, *input3);
    }
}

int main(int argc, char const *argv[])
{
    FILE *in, *out;
    char line[MAX_LENGTH];

    // Ensure the correct values were passed in
    if(argc != 3){
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    in = fopen(argv[1], "r");
        if( in==NULL ){
        fprintf(stderr,"Unable to open file %s for reading\n", argv[1]);
        exit(1);
    }

    out = fopen(argv[2], "w");
    if( out==NULL ){
        fprintf(stderr,"Unable to open file %s for writing\n", argv[2]);
        exit(1);
    }
    int w = 1;
    int *input1 = &w;
    int x = 1;
    int *input2 = &x;
    int y = 1;
    int *input3 = &y;
    int z = 1;
    int *throwaway = &z;

    fgets(line, MAX_LENGTH, in);

    // Get the size of the matricies, and the number of elements in each
    if(3 != sscanf(line, "%d %d %d %d\n", input1, input2, input3, throwaway)){
        fprintf(stderr,"%s is in incorrect format\n", argv[1]);
        exit(1);
    }

    int size = *input1;
    int aElements = *input2;
    int bElements = *input3;
            // If the result is not 0

    Matrix A = newMatrix(size);
    Matrix B = newMatrix(size);

    // Read in the empty line that follows the first
    fgets(line, MAX_LENGTH, in);

    readInMatrix(A, aElements, in, argv[1]);

    fgets(line, MAX_LENGTH, in);

    readInMatrix(B, bElements, in, argv[1]);

    // Print out A
    fprintf(out, "A has %d non-zero entries:\n", NNZ(A));
    printMatrix(out, A);

    // Print B
    fprintf(out, "\nB has %d non-zero entries:\n", NNZ(B));
    printMatrix(out, B);

    // Calculate A*1.5, then print it
    fprintf(out, "\n(1.5)*A =\n");
    Matrix mult = scalarMult(1.5, A);
    printMatrix(out, mult);
    freeMatrix(&mult);

    // Print A+B
    fprintf(out, "\nA+B =\n");
    Matrix addAB = sum(A, B);
    printMatrix(out, addAB);
    freeMatrix(&addAB);

    // A+A
    fprintf(out, "\nA+A =\n");
    Matrix addAA = sum(A, A);
    printMatrix(out, addAA);
    freeMatrix(&addAA);

    // B-A
    fprintf(out, "\nB-A =\n");
    Matrix diffBA = diff(B, A);
    printMatrix(out, diffBA);
    freeMatrix(&diffBA);

    // A-A. Could also print out an empty set instead, but I might as well do it properly
    fprintf(out, "\nA-A =\n");
    Matrix diffAA = diff(A, A);
    printMatrix(out, diffAA);
    freeMatrix(&diffAA);

    // Transpose of A
    fprintf(out, "\nTranspose(A) =\n");
    Matrix trans = transpose(A);
    printMatrix(out, trans);
    freeMatrix(&trans);

    // A*B
    fprintf(out, "\nA*B =\n");
    Matrix prod = product(A, B);
    printMatrix(out, prod);
    freeMatrix(&prod);

    // B*B
    fprintf(out, "\nB*B =\n");
    Matrix square = product(B, B);
    printMatrix(out, square);
    freeMatrix(&square);

    freeMatrix(&A);
    freeMatrix(&B);

    fclose(in);
    fclose(out);

    return 0;
}