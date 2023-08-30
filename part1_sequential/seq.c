#define Rows 8
#define Cols 8
#include <stdio.h>
#include <time.h>
#include <memory.h>
#include <omp.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "init.h"

void Row_Major_multiplication(int A[Rows][Cols],int B[Rows][Cols],int C[Rows][Cols]){
	int i , j , k;
    for ( i = 0; i < Rows; i++){
        for ( j = 0; j < Cols; j++){
            for ( k = 0; k < Rows; k++)
                C[i][j] = C[i][j] + A[i][k] * B[k][j];
        }
    }
    showMatrix_c(C);
}

void Col_Major_multiplication(int A[Rows][Cols],int Bcolumn[Rows*Cols],int C[Rows][Cols]){
	int i , j , k ;
    for ( i = 0; i < Rows; i++){
        for ( j = 0; j < Cols; j++){
            for ( k = 0; k < Rows; k++)
                C[i][j] = C[i][j] + A[i][k] * Bcolumn[j *Rows + k ];
        }
    }
    showMatrix_c(C);
}

int main() {
    int A[Rows][Cols];
    int B[Rows][Cols];
    int C[Rows][Cols];
    int Bcolumn[Rows*Cols];
    a_Initial(A);
    showMatrix_a(A);
    b_Initial(B);
    showMatrix_b(B);
    c_Initial(C);
    bcolumn_Initial(B , Bcolumn);
    double time_spent1 = 0.0;
    double time_spent2 = 0.0;
    clock_t begin , end;
    printf("sequential multiplication in row major case: \n");
    begin = clock();
    Row_Major_multiplication(A,B,C);
    end = clock();
    time_spent1 = (double)(end - begin) / CLOCKS_PER_SEC;
    c_Initial(C);
    printf("sequential multiplication in column major case: \n");
    begin = clock();
    Col_Major_multiplication(A,Bcolumn,C);
    end = clock();
    time_spent2 = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time spent in sequential multiplication in row major case =  %lf\n", time_spent1);
    printf("Time spent in sequential multiplication in column major case = %lf \n", time_spent2);

return 0;
}

