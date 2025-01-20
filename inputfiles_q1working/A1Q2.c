// ME5470 Introduction to Parallel Computing
// Assignment 1
// Vibhav Vaidya me21btech11054

// For the code to Work, the input files should be present in the same directory as this file.
// I have put one copy of Question2.c in the same directory as the input files, and one along with other files in hw1-Vibhav722 directory.
// Code will work only on the inputfiles Folder

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> 

// Epislon value for floating point comparison
#define epsilon 1e-10

// Get the value of n from the filename
int GetValue_n(const char *filename) {
    int num;
    sscanf(filename, "mat_%d.in", &num);
    return num;
}

// Function to multiply matrix with a vector
void matrix_vec_Mult(int n, double **Matrix, double *Vec, double *result) {
    for (int i = 0; i < n; i++) {
        result[i] = 0;
        for (int j = 0; j < n; j++) {
            result[i] += Matrix[i][j] * Vec[j];
        }
    }
}

// Fucntion to read input Matrix
void ReadMatrix(const char *filename, int n, double **matrix) {
    FILE *file = fopen(filename, "r");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(file, "%lf,", &matrix[i][j]);
        }
    }
    fclose(file);
}

// Fucntion to read input Vector
void ReadVector(const char *filename, int n, double *vector) {
    FILE *file = fopen(filename, "r");
    for (int i = 0; i < n; i++) {
        fscanf(file, "%lf,", &vector[i]);
    }
    fclose(file);
}

// Function to append eigenvalue to the vector file
void append_Eigenvalue(const char *filename, double eigenvalue) {
    FILE *file = fopen(filename, "a");
    fprintf(file, "%lf\n", eigenvalue);
    fclose(file);
}

// Function to check if the vector is an eigenvector of the matrix
void check_eigenvec(int n, double **Matrix, double *Vec, double *eigenvalue, int *flag) {
    
    // Perform matrix-vector multiplication and store it in result
    double *result = (double *)malloc(n * sizeof(double));
    matrix_vec_Mult(n, Matrix, Vec, result);

    // Found: 1 if non-zero element in Vec is found
    // Flag: 1 if the vector is an eigenvector

    int found = 0;
    double lambda = 0.0;
    for (int i = 0; i < n; i++) {
        if (fabs(Vec[i]) > epsilon) {
            lambda = result[i] / Vec[i];
            found = 1;
            break;
        }
    }

    if (!found) {
        *flag = 0;
        free(result);
        return;
    }

    // Initally assume the vector is an eigenvector
    // Check if Matrix*Vec = lambda*Vec within epsilon, else return flag = 0
    *flag = 1;
    for (int i = 0; i < n; i++) {
        if (fabs(result[i] - lambda * Vec[i]) > epsilon) {
            *flag = 0;
            break;
        }
    }
    
    // Store the lambda as eigenvalue if the vector is an eigenvector
    if (*flag) {
        *eigenvalue = lambda;
    }

    free(result);
}

int main() {
    
    // Hardcoding the matrix filenames, without input.in
    const char *Name_MatrixFiles[] = {
        "mat_000003.in",
        "mat_000005.in",
        "mat_000050.in",
        "mat_000080.in"
    };
    int Num_Matrix = 4;

    for (int m = 0; m < Num_Matrix; m++) {
        
        //Reading and memory allocation for the matrix and vector
        char Matrix_Name[50];
        strcpy(Matrix_Name, Name_MatrixFiles[m]);

        int n = GetValue_n(Matrix_Name);

        double **Matrix = (double **)malloc(n * sizeof(double *));
        for (int i = 0; i < n; i++) {
            Matrix[i] = (double *)malloc(n * sizeof(double));
        }
        double *Vector = (double *)malloc(n * sizeof(double));

        ReadMatrix(Matrix_Name, n, Matrix);

        for (int vecnum = 0; vecnum < 4; vecnum++) {
            
            char Vector_Name[50];
            sprintf(Vector_Name, "vec_%06d_%06d.in", n, vecnum + 1);

            FILE *file = fopen(Vector_Name, "r");
            if (file == NULL) {
                printf("vec %06d %06d.in : Not an eigenvector\n", n, vecnum + 1);
                continue; 
            }
            fclose(file);

            ReadVector(Vector_Name, n, Vector);

            double eigenvalue = 0.0;
            int flag = 0;

            check_eigenvec(n, Matrix, Vector, &eigenvalue, &flag);
            
            //Logic for vecnum+1: @vecnum=0, the vector will be vec_000001_000001, so +1 for offset
            if (flag) {
                printf("vec %06d %06d.in : Yes : %lf\n", n, vecnum + 1, eigenvalue);
                append_Eigenvalue(Vector_Name, eigenvalue);
            } else {
                printf("vec %06d %06d.in : Not an eigenvector\n", n, vecnum + 1);
            }
        }

        for (int i = 0; i < n; i++) {
            free(Matrix[i]);
        }
        free(Matrix);
        free(Vector);
    }

    return 0;
}