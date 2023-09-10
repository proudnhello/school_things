//Moore Macauley
//mmacule
//pa4

#include "Matrix.h"
#include "List.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef struct EntryObj* Entry;

typedef struct EntryObj
{
    int column;
    double value;
}EntryObj;

typedef struct MatrixObj
{
    List *rows;
    int size;
}MatrixObj;

// Creates a martix entry
Entry newEntry(int column, double value){
    Entry new = malloc(sizeof(EntryObj));
    assert(new != NULL);

    new->column = column;
    new->value = value;
    return new;
}

// Frees a matrix entry
void freeEntry(Entry *E){
    free(*E);
    *E = NULL;
}

// Creates a new Matrix
Matrix newMatrix(int n){
    if(n < 0){
        return NULL;
    }
    Matrix new = malloc(sizeof(MatrixObj));
    assert(new != NULL);

    new->size = n;

    // Allocate space for n+1 rows b/c row 0 is unused
    new->rows = calloc(n+1, sizeof(List));
    assert(new->rows != NULL);

    // Fill up all the rows except 0 with Lists
    for(int i = 1; i <= n; i++){
        new->rows[i] = newList();
    }

    return new;
}

// Frees a matrix
void freeMatrix(Matrix* pM){
    if(pM == NULL || *pM == NULL){
        return;
    }
    Matrix old = *pM;
    // remove all entries from the matrix
    makeZero(old);
    for(int i = 1; i <= old->size; i++){
        freeList(&(old->rows[i]));
    }
    free(old->rows);
    free(old);
    old = NULL;
}

// Returns n in an nxn matrix
int size(Matrix M){
    if(M == NULL){
        fprintf(stderr, "Matrix Error: Calling length() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    return M->size;
}

// Returns the number of non zero elements
int NNZ(Matrix M){
    if(M == NULL){
        fprintf(stderr, "Matrix Error: Calling NNZ() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    int total = 0;
    // Simply sum the number of elements every list has
    // While I could track NNZ in the MatrixObj struct, this was easier, albiet less efficient
    for(int i = 1; i <= M->size; i++){
        total += length(M->rows[i]);
    }
    return total;
}

// Returns true if A and B are equal, false otherwise
int equals(Matrix A, Matrix B){
    if(A == NULL || B == NULL){
        fprintf(stderr, "Matrix Error: Calling equals() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    // If they have different sizes, they are obviously not equal
    if(A->size != B->size){
        return 0;
    }
    // If they point to the same bit of memory, they are obviously equal
    if(A == B){
        return 1;
    }

    // Iterate over all the rows
    for(int i = 1; i <= A->size; i++){
        List ARow = A->rows[i];
        List BRow = B->rows[i];

        moveFront(ARow);
        moveFront(BRow); 
        while(index(ARow) != -1 && index(BRow) != -1){
            Entry ACurr = get(ARow);
            Entry BCurr = get(BRow);
            // If an entry is not equal, return false
            if(ACurr->value != BCurr->value || ACurr->column != BCurr->column){
                return 0;
            }
            moveNext(ARow);
            moveNext(BRow);
        }
        // If the end of one row is reached, but not the other, return false
        if(index(ARow) != -1 || index(BRow) != -1){
                return 0;
        }
    }
    // If no reason for the result to be false is found, return true
    return 1;
}

void makeZero(Matrix M){
    if(M == NULL){
        fprintf(stderr, "Matrix Error: Calling makeZero() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    
    for(int i = 1; i <= M->size; i++){
        moveFront(M->rows[i]);
        while (index(M->rows[i]) != -1){

            // Because freeList uses void pointers, all of the entries need to be freed sperately
            Entry freeable = get(M->rows[i]);
            freeEntry(&freeable);
            moveNext(M->rows[i]);
        }
        clear(M->rows[i]);
    }
}

// Sets row i, column j to x
void changeEntry(Matrix M, int i, int j, double x){
    if(M == NULL){
        fprintf(stderr, "Matrix Error: Calling changeEntry() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    if(i < 1 || i > M->size || j < 1 || j > M->size){
        fprintf(stderr, "Matrix Error: Calling changeEntry() when (i, j) is out of bounds\n");
        exit(EXIT_FAILURE);
    }

    // Get row i, and iterate over it
    List row = M->rows[i];
    moveFront(row);

    while(index(row) != -1){
        Entry current = (Entry)get(row);

        // If the (i, j) already has a value, change it to x
        if(current->column == j){
            if(x == 0.0){
                Entry holder = get(row);
                freeEntry(&holder);
                delete(row);
                return;
            }
            current->value = x;
            return;

        // Otherwise, find the first largest column in row i, and add the new entry before
        }else if (current->column > j){
            if(x != 0.0){
                insertBefore(row, newEntry(j, x));
            }
            return;
        }
        moveNext(row);
    }

    // If there were no largest column, add the new entry to the end
    if(x != 0.0){
        append(row, newEntry(j, x));
    }
    
}

// return a copy of M
Matrix copy(Matrix M){
    if(M == NULL){
        fprintf(stderr, "Matrix Error: Calling copy() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }

    Matrix new = newMatrix(M->size);

    // Iterate over all of the rows
    for(int i = 1; i <= M->size; i++){
        List oldRow = M->rows[i];
        List newRow = new->rows[i];
        moveFront(oldRow);
        while(index(oldRow) != -1){

            // For every entry in the old matrix, append a copy to the new matrix
            // changeEntry is not used b/c it has a runtime of theta(a), while append has a runtime of theta(1)
            Entry current = get(oldRow);
            append(newRow, newEntry(current->column, current->value));
            moveNext(oldRow);
        }
    }
    return new;
}

// Returns the transpose of A
Matrix transpose(Matrix A){
    if(A == NULL){
        fprintf(stderr, "Matrix Error: Calling transpose() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }

    Matrix new = newMatrix(A->size);

    // Iterate over all of the rows
    for(int i = 1; i <= A->size; i++){
        List oldRow = A->rows[i];
        moveFront(oldRow);
        while(index(oldRow) != -1){

            // For every entry in the old matrix, append a copy to the appropriate transposed row 
            // changeEntry is not used b/c it has a runtime of theta(a), while append has a runtime of theta(1)
            Entry current = get(oldRow);
            append(new->rows[current->column], newEntry(i, current->value));
            moveNext(oldRow);
        }
    }
    return new;
}

// Returns A * x
Matrix scalarMult(double x, Matrix A){
    if(A == NULL){
        fprintf(stderr, "Matrix Error: Calling scalarMult() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }

    Matrix new = newMatrix(A->size);

    // Iterate over all of the rows
    for(int i = 1; i <= A->size; i++){
        List oldRow = A->rows[i];
        moveFront(oldRow);
        while(index(oldRow) != -1){

            // For every entry in the old matrix, append a copy with the value *x
            // changeEntry is not used b/c it has a runtime of theta(a), while append has a runtime of theta(1)
            Entry current = get(oldRow);
            append(new->rows[i], newEntry(current->column, (current->value)*x));
            moveNext(oldRow);
        }
    }
    return new;
}

// Adds B * multiplier to A. For the multiplier, -1 is used for subtraction, 1 for addition
Matrix sumAndDiff(Matrix A, Matrix B, int multiplier){

    if(A == B){
        // As A+A == 2*A, just do that instead
        if(multiplier == 1){
            return scalarMult(2, A);
        // As A-A == 0, just return an empty matrix 
        }else{
            return newMatrix(A->size);
        }
    }

    Matrix new = newMatrix(A->size);

    // Iterate over all of the rows
    for(int i = 1; i <= A->size; i++){
        List ARow = A->rows[i];
        List BRow = B->rows[i];
        moveFront(ARow);
        moveFront(BRow);

        // Iterate over each row
        while(index(ARow) != -1 && index(BRow) != -1){
            Entry ACurr = get(ARow);
            Entry BCurr = get(BRow);
            
            // If the columns are the same, then append the sum of the two values, and move to the next value in both
            if(ACurr->column == BCurr->column){
                double value = ACurr->value + (BCurr->value*multiplier);
                if(value != 0){
                    append(new->rows[i], newEntry(ACurr->column, value));
                }
                moveNext(ARow);
                moveNext(BRow);
            }

            // If A has a smaller column, then append that value to the new matrix, and increment the A row
            else if (ACurr->column < BCurr->column){
                append(new->rows[i], newEntry(ACurr->column, ACurr->value));
                moveNext(ARow);

            // Otherwise, append the B value to the new matrix, and increment the B row
            }else{
                append(new->rows[i], newEntry(BCurr->column, BCurr->value * multiplier));
                moveNext(BRow);
            }
            
        }

        // Once we reach the end of one row, append whatever is left of the other row to the new matrix
        while(index(ARow) != -1){
            Entry ACurr = get(ARow);
            append(new->rows[i], newEntry(ACurr->column, ACurr->value));
            moveNext(ARow);
        }
        while(index(BRow) != -1){
            Entry BCurr = get(BRow);
            append(new->rows[i], newEntry(BCurr->column, BCurr->value * multiplier));
            moveNext(BRow);
        }
    }
    
    return new;
}

// Adds A and B
Matrix sum(Matrix A, Matrix B){
    if(A == NULL || B == NULL){
        fprintf(stderr, "Matrix Error: Calling sum() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    if(A->size != B->size){
        fprintf(stderr, "Matrix Error: Calling sum() on different sizes\n");
        exit(EXIT_FAILURE);
    }

    return sumAndDiff(A, B, 1);

}

// Subtracts B from A
Matrix diff(Matrix A, Matrix B){
    if(A == NULL || B == NULL){
        fprintf(stderr, "Matrix Error: Calling diff() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    if(A->size != B->size){
        fprintf(stderr, "Matrix Error: Calling diff() on different sizes\n");
        exit(EXIT_FAILURE);
    }

    return sumAndDiff(A, B, -1);
}

// Finds the vector dot product of P and Q
double vectorDot(List P, List Q){
    double total = 0.0;

    moveFront(P);
    moveFront(Q);
    while(index(P) != -1 && index(Q) != -1){
        Entry PItem = get(P);
        Entry QItem = get(Q);
        // For every column where both P and Q have a value, increment the total by the product
        if(QItem->column == PItem->column){
            total += PItem->value * QItem->value;
            moveNext(P);
            moveNext(Q);
        }else if (QItem->column < PItem->column){
            moveNext(Q);
        }else if (PItem->column < QItem->column){
            moveNext(P);
        }
        
    }

    return total;
}

// Finds the product of A and B
Matrix product(Matrix A, Matrix B){
    if(A == NULL || B == NULL){
        fprintf(stderr, "Matrix Error: Calling product() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    if(A->size != B->size){
        fprintf(stderr, "Matrix Error: Calling product() on different sizes\n");
        exit(EXIT_FAILURE);
    }

    // Get the transpose of B, so that the rows of trans are the columns of B
    Matrix trans = transpose(B);
    Matrix new = newMatrix(A->size);

    // Iterate over all the values of A and B
    for(int i = 1; i <= A->size; i++){
        for(int j = 1; j <= trans->size; j++){

            // For every position in the matrix, find the vector dot product of the corrisponding values
            List ARow = A->rows[i];
            List BColumn = trans->rows[j];
            double value = vectorDot(ARow, BColumn);

            // If the result is not 0, add it to the matrix
            if(value != 0.0){
                append(new->rows[i], newEntry(j, value));
            }
        }
    }

    freeMatrix(&trans);

    return new;
}

// Prints out a matrix, 0s go unprinted, with entire rows consisting of 0s being unprinted
void printMatrix(FILE* out, Matrix M){
    if(M == NULL){
        fprintf(stderr, "Matrix Error: Calling printMatrix() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }

    // Iterate over all of the rows
    for(int i = 1; i <= M->size; i++){
        List row = M->rows[i];
        // If the row has a non 0 entry
        if (length(row) != 0){
            fprintf(out, "%d:", i);
            moveFront(row);
            // Iterate over the row, printing the (row, value) tuple for every entry
            while(index(row) != -1){
                Entry current = (Entry)get(row);
                fprintf(out, " (%d, %.1f)", current->column, current->value);
                moveNext(row);
            }
            fprintf(out, "\n");
        }
        
    }
}