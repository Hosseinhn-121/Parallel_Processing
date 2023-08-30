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

void Row_Major_multiplication(int A[Rows][Cols],int B[Rows][Cols],int c[Rows*Cols/32],int rank){
	int i , j , k , x;
    int start1=floor(rank/4);
    int end1=floor(rank/4) + 1;
    int start2=(rank % 4) * 2;
    int end2=(rank % 4) * 2 + 2;
    int a[Rows][Cols];
    int b[Rows][Cols];
    if(rank != 0){
        MPI_Status status;
        MPI_Recv(&a, Rows*Cols, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, Rows*Cols, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
        for (i = start1; i < end1; i++){
            for (j = start2, x = 0; j < end2; j++,x++){
                for (k = 0; k < Rows; k++){
                    c[x] = c[x] + a[i][k] * b[k][j];
                }
            }

        }
    }

    if(rank == 0){
        for (i = start1; i < end1; i++){
            for (j = start2,x = 0; j < end2; j++,x++){
                for (k = 0; k < Rows; k++){
                    c[x] = c[x] + A[i][k] * B[k][j];
                }
            }

        }
    }
}

void Col_Major_multiplication(int A[Rows][Cols],int Bcolumn[Rows*Cols],int c[Rows*Cols/32],int rank){
	int i , j , k , x ;
    int start1=floor(rank/4);
    int end1=floor(rank/4) + 1;
    int start2=(rank % 4) * 2;
    int end2=(rank % 4) * 2 + 2;
    int a[Rows][Cols];
    int bcolumn[Rows*Cols];
    if(rank != 0){
        MPI_Status status;
        MPI_Recv(&a, Rows*Cols, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&bcolumn, Rows*Cols, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
        for (i = start1; i < end1; i++){
            for (j = start2, x = 0; j < end2; j++,x++){
                for (k = 0; k < Rows; k++){
                    c[x] = c[x] + a[i][k] * bcolumn[j * Rows + k ];
                }
            }

        }
    }

    if(rank == 0){
        for (i = start1; i < end1; i++){
            for (j = start2,x = 0; j < end2; j++,x++){
                for (k = 0; k < Rows; k++){
                    c[x] = c[x] + A[i][k] * Bcolumn[j * Rows + k ];
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
    int c[Rows*Cols/32]={0};
    int pre_C[Rows*Cols] ={0};
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
        printf("parallel multiplication by MPI in row major case: \n");
        begin = clock();
    }
    Row_Major_multiplication(A,B,c,rank);
    MPI_Gather(c, Rows*Cols/32, MPI_INT,pre_C, Rows*Cols/32,  MPI_INT, 0, MPI_COMM_WORLD);
    if(rank == 0){
        int i,j;
        for (i = 0; i < Rows; i++){
            for (j = 0; j < Rows; j++){
                C[i][j] = pre_C[i * Rows +j];
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
        printf("parallel multiplication by MPI in column major case: \n");
        begin = clock();
    }
    Col_Major_multiplication(A,Bcolumn,c,rank);
    MPI_Gather(c, Rows*Cols/32, MPI_INT,pre_C, Rows*Cols/32,  MPI_INT, 0, MPI_COMM_WORLD);
    if(rank == 0){
        int i,j;
        for (i = 0; i < Rows; i++){
            for (j = 0; j < Rows; j++){
                C[i][j] = pre_C[i * Rows +j];
            }
        }
        showMatrix_c(C);
        end = clock();
        time_spent2 = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Time spent in parallel multiplication by MPI in row major case =  %lf\n", time_spent1);
        printf("Time spent in parallel multiplication by MPI in column major case = %lf \n", time_spent2);
    }
    MPI_Finalize();

return 0;
}

