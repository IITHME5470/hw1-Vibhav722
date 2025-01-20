// ME5470 Introduction to Parallel Computing
// Assignment 1
// Vibhav Vaidya me21btech11054


#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void print_to_file(int n, double **A, int format_flag){
    char filename[50];
    
    // For asc/bin format
    char string_format[4];

    if(format_flag == 0){
        snprintf(string_format, sizeof(string_format), "asc");
    } else if(format_flag == 1)
    {
        snprintf(string_format, sizeof(string_format), "bin");
    }
    else{
        printf("Invalid format flag\n");
        return;
    }

    snprintf(filename, sizeof(filename), "array_%06d_%s.out", n, string_format);
    
    // Writing contents to file
    if(format_flag == 0){
        FILE *file = fopen(filename, "w");
        if(file == NULL){
            printf("Error: Could not open file %s\n", filename);
            return;
        }

        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                fprintf(file, "%lf ", A[i][j]);
            }
            fprintf(file, "\n");
        }

        fclose(file);
    } else if(format_flag == 1){
        FILE *file = fopen(filename, "wb");
        if(file == NULL){
            printf("Error: Could not open file %s\n", filename);
            return;
        }

        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                fwrite(&A[i][j], sizeof(double), 1, file);
            }
        }

        fclose(file);
    }
}

int main(){
    int n = 4000;
    int format_flag;
    
    // Memory allocation, and initialization of matrix
    double **A = (double **)malloc(n * sizeof(double *));
    for(int i=0;i<n;i++){
        A[i] = (double *)malloc(n * sizeof(double));
    }

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            A[i][j] = i+j;
        }
    }

    format_flag = 0;
    print_to_file(n, A, format_flag);


    

    for(int i=0;i<n;i++){
        free(A[i]);
    }
    free(A);

    return 0;

}