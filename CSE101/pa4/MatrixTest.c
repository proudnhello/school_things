//Moore Macauley
//mmacule
//pa4
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include"List.h"
#include"Matrix.h"

int main(int argc, char* argv[]){
   
    Matrix test = newMatrix(10);
    Matrix test2 = newMatrix(10);
    printf("%d\n", size(test));
    printMatrix(stdout, test);
    printf("%d\n", NNZ(test));
    printf("%d\n", equals(test, test2));

    for(int i = 1; i <= 10; i++){
        for( int j = 1; j <= 10; j++){
            if(i != 5 && j != 5){
                changeEntry(test, i, j, (double)(i + j));
                changeEntry(test2, i, j, (double)(i + j));
            }
        }
    }

    printf("----------------------------------------------------------------\n\n");

    printf("%d\n", size(test));
    printMatrix(stdout, test);
    printf("%d\n", NNZ(test));

    printf("----------------------------------------------------------------\n\n");

    for(int i = 1; i <= 10; i++){
        for( int j = 1; j <= 10; j++){
            if(i != 10 && j != 10){
                changeEntry(test, i, j, (double)(i * j));
                changeEntry(test2, i, j, (double)(i * j));
            }
        }
    }

    printf("%d\n", size(test));
    printMatrix(stdout, test);
    printf("%d\n", NNZ(test));

    printMatrix(stdout, test2);

    printf("%d\n", equals(test, test2));
    changeEntry(test2, 10, 10, 67.725783472);
    printf("%d\n", equals(test, test2));

    printf("----------------------------------------------------------------\n\n");

    makeZero(test);

    printMatrix(stdout, test);

    printf("----------------------------------------------------------------\n\n");

    Matrix test3 = copy(test2);
    printMatrix(stdout, test2);
    printf("\n");
    printMatrix(stdout, test3);
    printf("%d\n", equals(test2, test3));

    printf("----------------------------------------------------------------\n\n");

    Matrix test4 = newMatrix(10);

    for(int i = 1; i <= 10; i++){
        for( int j = 1; j <= 6; j+=2){
            changeEntry(test4, i, j, (double)(i * j));
        }
    }
    
    Matrix test5 = transpose(test4);

    printMatrix(stdout, test4);
    printf("\n");
    printMatrix(stdout, test5);

    printf("----------------------------------------------------------------\n\n");

    Matrix test6 = scalarMult(25, test5);
    printMatrix(stdout, test6);

    printf("----------------------------------------------------------------\n\n");

    Matrix test7 = sum(test4, test5);
    printMatrix(stdout, test4);
    printf("\n");
    printMatrix(stdout, test5);
    printf("\n");
    printMatrix(stdout, test7);

    printf("----------------------------------------------------------------\n\n");
    Matrix test8 = diff(test4, test6);
    printMatrix(stdout, test4);
    printf("\n");
    printMatrix(stdout, test6);
    printf("\n");
    printMatrix(stdout, test8);

    printf("----------------------------------------------------------------\n\n");
    Matrix test9 = diff(test2, test3);
    printMatrix(stdout, test2);
    printf("\n");
    printMatrix(stdout, test3);
    printf("\nThere should be nothing here\n");
    printMatrix(stdout, test9);

    printf("----------------------------------------------------------------\n\n");

    Matrix smallerSoICanFindProductByHand = newMatrix(3);
    Matrix smallerSoICanFindProductByHand2 = newMatrix(3);

    changeEntry(smallerSoICanFindProductByHand, 1, 1, 1);
    changeEntry(smallerSoICanFindProductByHand, 2, 2, 2);
    changeEntry(smallerSoICanFindProductByHand, 3, 3, 3);

    changeEntry(smallerSoICanFindProductByHand2, 1, 2, 45);
    changeEntry(smallerSoICanFindProductByHand2, 3, 2, 6);
    changeEntry(smallerSoICanFindProductByHand2, 2, 1, 8);
    changeEntry(smallerSoICanFindProductByHand2, 3, 3, 6);
    printMatrix(stdout, smallerSoICanFindProductByHand);
    printf("\n");
    printMatrix(stdout, smallerSoICanFindProductByHand2);
    printf("\n");
    printMatrix(stdout, product(smallerSoICanFindProductByHand, smallerSoICanFindProductByHand2));

    freeMatrix(&test);
    freeMatrix(&test2);
    freeMatrix(&test3);
    freeMatrix(&test4);
    freeMatrix(&test5);
    freeMatrix(&test6);
    freeMatrix(&test7);
    freeMatrix(&test8);
    freeMatrix(&test9);
    freeMatrix(&smallerSoICanFindProductByHand2);
    freeMatrix(&smallerSoICanFindProductByHand);
}