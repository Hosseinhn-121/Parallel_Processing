#define Rows 8
#define Cols 8
#include <stdio.h>
#include <time.h>
#include <memory.h>
#include <omp.h>
#include <string.h>
#include <math.h>
#include <mpi.h>
#include "init.h"

void Row_Major_multiplication(int A[Rows][Cols],int B[Rows][Cols],int c[Rows*Cols/16],int rank){
	int i , j , k , x=0;
    int start1=floor(rank/Rows) * 2;
    int end1=floor(rank/Rows) * 2  + 2;
    int start2=floor((rank % Rows)/2) * 2;
    int end2=floor((rank % Rows)/2) * 2 + 2;
    int start3=(rank % 2) * 4;
    int end3=(rank % 2) * 4 + 4;
    int a[Rows][Cols];
    int b[Rows][Cols];
    if(rank != 0){
        MPI_Status status;
        MPI_Recv(&a, Rows*Cols, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, Rows*Cols, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
        for (i = start1; i < end1; i = i + 2){
            for (j = start2; j < end2; j = j + 2){
                for (k = start3; k < end3; k = k + 2){
                    c[x] = c[x]+a[i][k]*b[k][j]+ a[i][k+1] * b[k+1][j];
                    c[x+1] = c[x+1]+a[i][k]*b[k][j+1]+ a[i][k+1] * b[k+1][j+1];
                    c[x+2] = c[x+2]+a[i+1][k]*b[k][j]+ a[i+1][k+1] * b[k+1][j];
                    c[x+3] = c[x+3]+a[i+1][k]*b[k][j+1]+ a[i+1][k+1] * b[k+1][j+1];
                }
            }
        }
    }
    if(rank == 0){
        for (i = start1; i < end1; i = i + 2){
            for (j = start2; j < end2; j = j + 2){
                for (k = start3; k < end3; k = k + 2){
                    c[x] = c[x]+A[i][k]*B[k][j]+ A[i][k+1] * B[k+1][j];
                    c[x+1] = c[x+1]+A[i][k]*B[k][j+1]+ A[i][k+1] * B[k+1][j+1];
                    c[x+2] = c[x+2]+A[i+1][k]*B[k][j]+ A[i+1][k+1] * B[k+1][j];
                    c[x+3] = c[x+3]+A[i+1][k]*B[k][j+1]+ A[i+1][k+1] * B[k+1][j+1];
                }
            }
        }
    }
}

void Col_Major_multiplication(int A[Rows][Cols],int Bcolumn[Rows*Cols],int c[Rows*Cols/16],int rank){
	int i , j , k , x=0 ;
    int start1=floor(rank/Rows) * 2;
    int end1=floor(rank/Rows) * 2  + 2;
    int start2=floor((rank % Rows)/2) * 2;
    int end2=floor((rank % Rows)/2) * 2 + 2;
    int start3=(rank % 2) * 4;
    int end3=(rank % 2) * 4 + 4;
    int a[Rows][Cols];
    int b[Rows*Cols];
    if(rank != 0){
        MPI_Status status;
        MPI_Recv(&a, Rows*Cols, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, Rows*Cols, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
        for (i = start1; i < end1; i = i + 2){
            for (j = start2; j < end2; j = j + 2){
                for (k = start3; k < end3; k = k + 2){
                    c[x] = c[x]+ a[i][k] * b[j * Rows + k]+ a[i][k+1] * b[j * Rows + k + 1];
                    c[x+1] = c[x+1]+a[i][k] * b[(j+1) * Rows + k]+ a[i][k+1] * b[(j+1) * Rows + k +1];
                    c[x+2] = c[x+2]+a[i+1][k] * b[j * Rows + k]+ a[i+1][k+1] * b[j * Rows + k + 1];
                    c[x+3] = c[x+3]+a[i+1][k] * b[(j+1) * Rows + k]+ a[i+1][k+1] * b[(j+1) * Rows + k +1];
                }
            }
        }
    }

    if(rank == 0){
       for (i = start1; i < end1; i = i + 2){
            for (j = start2; j < end2; j = j + 2){
                for (k = start3; k < end3; k = k + 2){
                    c[x] = c[x]+A[i][k] * Bcolumn[j * Rows + k]+ A[i][k+1] * Bcolumn[j * Rows + k + 1];
                    c[x+1] = c[x+1]+A[i][k] * Bcolumn[(j+1) * Rows + k ]+ A[i][k+1] * Bcolumn[(j+1) * Rows + k +1];
                    c[x+2] = c[x+2]+A[i+1][k] * Bcolumn[j * Rows + k]+ A[i+1][k+1] * Bcolumn[j * Rows + k + 1];
                    c[x+3] = c[x+3]+A[i+1][k] * Bcolumn[(j+1) * Rows + k ]+ A[i+1][k+1] * Bcolumn[(j+1) * Rows + k +1];
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int num_worker, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_worker);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int A[Rows][Cols];
    int B[Rows][Cols];
    int C[Rows][Cols];
    int Bcolumn[Rows*Cols];
    int c[Rows*Cols/16]={0};
    int pre_C[Rows*Cols*2] ={0};
    double time_spent1 = 0.0;
    double time_spent2 = 0.0;
    clock_t begin , end;
    if(rank == 0){
        a_Initial(A);
        showMatrix_a(A);
        b_Initial(B);
        showMatrix_b(B);
        c_Initial(C);
        bcolumn_Initial(B , Bcolumn);
        int p_numb;
        for(p_numb = 1; p_numb< num_worker; p_numb++){
            MPI_Send(&A, Rows*Cols, MPI_INT, p_numb, 1, MPI_COMM_WORLD);
            MPI_Send(&B, Rows*Cols, MPI_INT, p_numb, 2, MPI_COMM_WORLD);
        }
        printf("devided parallel multiplication by MPI in row major case: \n");
        begin = clock();   
    }
    Row_Major_multiplication(A,B,c,rank);
    MPI_Gather(c, Rows*Cols/16, MPI_INT,pre_C, Rows*Cols/16,  MPI_INT, 0, MPI_COMM_WORLD);
    if(rank == 0){
        int i,j ,x = 0;
        for (i = 0; i < Rows; i = i +2){
            for (j = 0; j < Cols; j = j + 2){
                C[i][j] = pre_C[x] + pre_C[x + 4];
                C[i][j+1] = pre_C[x + 1] + pre_C[x + 5];
                C[i+1][j] = pre_C[x + 2] + pre_C[x + 6];
                C[i+1][j+1] = pre_C[x + 3] + pre_C[x + 7];
                x = x + 8;
            }
        }
        showMatrix_c(C);
        end = clock();
        time_spent1 = (double)(end - begin) / CLOCKS_PER_SEC;
    }
    c_Initial(C);
    memset(c,0,sizeof c);
    memset(pre_C,0,sizeof pre_C);
    if(rank == 0){
        int p_numb;
        for(p_numb = 1; p_numb< num_worker; p_numb++){
            MPI_Send(&A, Rows*Cols, MPI_INT, p_numb, 1, MPI_COMM_WORLD);
            MPI_Send(&Bcolumn, Rows*Cols, MPI_INT, p_numb, 2, MPI_COMM_WORLD);
        }
        printf("devided parallel multiplication by MPI in column major case: \n");
        begin = clock();
    }
    Col_Major_multiplication(A,Bcolumn,c,rank);
    MPI_Gather(c, Rows*Cols/16, MPI_INT,pre_C, Rows*Cols/16,  MPI_INT, 0, MPI_COMM_WORLD);
    if(rank == 0){
        int i,j ,x = 0;
        for (i = 0; i < Rows; i = i +2){
            for (j = 0; j < Cols; j = j + 2){
                C[i][j] = pre_C[x] + pre_C[x + 4];
                C[i][j+1] = pre_C[x + 1] + pre_C[x + 5];
                C[i+1][j] = pre_C[x + 2] + pre_C[x + 6];
                C[i+1][j+1] = pre_C[x + 3] + pre_C[x + 7];
                x = x + 8;
            }
        }
        showMatrix_c(C);
        end = clock();
        time_spent2 = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Time spent in devided parallel multiplication by MPI in row major case =  %lf\n", time_spent1);
        printf("Time spent in devided parallel multiplication by MPI in column major case = %lf \n", time_spent2);
    }
    MPI_Finalize();

return 0;
}

