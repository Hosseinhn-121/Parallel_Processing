static int i,j,x;
void a_Initial(int A[Rows][Cols]){
    for (i = 0; i < Rows; i++){
        for (j = 0,x = 0; j < Cols; j++){
            if (j + i < 8){
                A[i][j] = j + i + 1;
            } 
            else{
                A[i][j] = x + 1;
                x++;
            }
        }
    }
}

void b_Initial(int B[Rows][Cols]){
    for (i = 0; i < Rows; i++){
        for (j = 0,x = 0; j < Cols; j++){
            if (j + i < 8){
                B[i][j] = 8 - j - i;
            } 
            else{
                B[i][j] = 8 - x;
                x++;
            }
        }
    }
}

void c_Initial(int C[Rows][Cols]){
    for(i = 0; i < Rows; i++){
        for(j = 0; j< Cols; j++){
            C[i][j] = 0 ;
        }
    }
}

void bcolumn_Initial(int B[Rows][Cols] , int Bcolumn[Rows*Cols]){
    for (i = 0; i < Rows; i++){
        for (j = 0,x = 0; j < Cols; j++){
            if (j + i < 8){
                Bcolumn[i * Rows + j] = 8 - j - i;
            } 
            else{
                Bcolumn[i * Rows + j] = 8 - x;
                x++;
            }
        }
    }
}

void sa_Initial(int A[Rows][Cols]){
    for (i = 0; i < Rows; i++){
        for (j = 0,x = 0; j < Cols; j++){
            if (j + (i % 8) < 8){
                A[i][j] = (j % 8) + (i % 8) + 1;
            } 
            else{
                A[i][j] = x + 1;
                x++;
                if(x == 8)
                    x = 0;
            }
        }
    }
}

void sb_Initial(int B[Rows][Cols]){
    for (i = 0; i < Rows; i++){
        for (j = 0,x = 0; j < Cols; j++){
            if (j + (i % 8) < 8){
                B[i][j] = 8 - (j % 8) - (i % 8);
            } 
            else{
                B[i][j] = 8 - x;
                x++;
                if(x == 8)
                    x = 0;
            }
        }
    }
}

void sbcolumn_Initial(int B[Rows][Cols] , int Bcolumn[Rows*Cols]){
    for (i = 0; i < Rows; i++){
        for (j = 0,x = 0; j < Cols; j++){
            if (i % 8 + j  < 8){
                Bcolumn[i * Rows + j] = 8 - (j % 8) - (i % 8);
            } 
            else{
                Bcolumn[i * Rows + j] = 8 - x;
                x++;
                if(x == 8)
                    x = 0;
            }
        }
    }
}

void showMatrix_a(int A[Rows][Cols]){
    printf("initialized matrix A : \n");
        if(Rows == 8){
            for (i = 0; i < Rows; i++){
                for (j = 0; j < Cols; j++){
                    printf("%d ", A[i][j]);
                }
                printf("\n");
            }
            printf("\n");
        }
        else{
            for ( i = 0; i < (Rows/128); i++){
                for ( j = 0; j < (Cols/128); j++){
                    printf("%d ", A[i][j]);
                }
                printf("\n");
            }
            printf("\n");
        }
}
      

void showMatrix_b(int B[Rows][Cols]){
    printf("initialized matrix B : \n");
    if(Rows == 8){
        for (i = 0; i < Rows; i++){
            for (j = 0; j < Cols; j++){
                printf("%d ", B[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
    else{
        for ( i = 0; i < (Rows/128); i++){
            for ( j = 0; j < (Cols/128); j++){
                printf("%d ", B[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
}      

void showMatrix_c(int C[Rows][Cols]){
    printf("multiplication result in matrix C : \n");
    if(Rows == 8){
        for (i = 0; i < Rows; i++){
            for (j = 0; j < Cols; j++){
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
    else{
        for ( i = 0; i < (Rows/128); i++){
            for ( j = 0; j < (Cols/128); j++){
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
}      
                