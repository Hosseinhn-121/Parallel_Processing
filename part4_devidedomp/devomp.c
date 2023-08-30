#define Rows 8
#define Cols 8
#include <stdio.h>
#include <time.h>
#include <memory.h>
#include <omp.h>
#include <string.h>
#include <math.h>
#include "init.h"

void Row_Major_multiplication(int A[Rows][Cols],int B[Rows][Cols],int C[Rows][Cols]){
	int i , j , k;
    omp_set_num_threads(32);
#pragma omp parallel for collapse(3)
    for (i = 0; i < Rows; i = i+2){
        for (j = 0; j < Cols; j= j +2){
            for (k = 0; k < Rows; k=k+2){
#pragma omp critical
                C[i][j] = C[i][j] + A[i][k] * B[k][j]+ A[i][k+1] * B[k+1][j];
                C[i][j+1] = C[i][j+1] + A[i][k] * B[k][j+1]+ A[i][k+1] * B[k+1][j+1];
                C[i+1][j] = C[i+1][j] + A[i+1][k] * B[k][j]+ A[i+1][k+1] * B[k+1][j];
                C[i+1][j+1] = C[i+1][j+1] + A[i+1][k] * B[k][j+1]+ A[i+1][k+1] * B[k+1][j+1];
            }

        }
    }
    showMatrix_c(C);
}

void Col_Major_multiplication(int A[Rows][Cols],int Bcolumn[Rows*Cols],int C[Rows][Cols]){
    int i , j , k ;
    omp_set_num_threads(32);
#pragma omp parallel for collapse(3)
    for (i = 0; i < Rows; i = i+2){
        for (j = 0; j < Cols; j= j +2){
            for (k = 0; k < Rows; k=k+2){
#pragma omp critical
                C[i][j] = C[i][j]+A[i][k]*Bcolumn[j*Rows+k]+ A[i][k+1] * Bcolumn[j*Rows+k+1];
                C[i][j+1] = C[i][j+1]+A[i][k]*Bcolumn[(j+1)*Rows+k]+ A[i][k+1] * Bcolumn[(j+1)*Rows+k+1];
                C[i+1][j] = C[i+1][j]+A[i+1][k]*Bcolumn[j*Rows+k]+ A[i+1][k+1] * Bcolumn[j*Rows+k+1];
                C[i+1][j+1] = C[i+1][j+1]+A[i+1][k]*Bcolumn[(j+1)*Rows+k]+ A[i+1][k+1] * Bcolumn[(j+1)*Rows+k+1];
            }   
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
    printf("devided parallel multiplication by OMP in row major case: \n");
    begin = clock();
    Row_Major_multiplication(A,B,C);
    end = clock();
    time_spent1 = (double)(end - begin) / CLOCKS_PER_SEC;
    c_Initial(C);
    printf("devided parallel multiplication by OMP in column major case: \n");
    begin = clock();
    Col_Major_multiplication(A,Bcolumn,C);
    end = clock();
    time_spent2 = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time spent in devided parallel multiplication by OMP in row major case =  %lf\n", time_spent1);
    printf("Time spent in devided parallel multiplication by OMP in column major case = %lf \n", time_spent2);
return 0;
}

