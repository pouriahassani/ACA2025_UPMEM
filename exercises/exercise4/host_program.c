/**
 * Exercise 4: Matrix Operations - Host Program
 * 
 * TODO: Complete this challenging matrix-vector multiplication program
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dpu.h>

#define NR_DPUS 1
#define MATRIX_ROWS 128
#define MATRIX_COLS 64
#define MATRIX_SIZE (MATRIX_ROWS * MATRIX_COLS)

// CPU reference implementation
void cpu_matrix_vector_multiply(uint32_t *matrix, uint32_t *vector, uint32_t *result) {
    for (uint32_t row = 0; row < MATRIX_ROWS; row++) {
        uint32_t sum = 0;
        for (uint32_t col = 0; col < MATRIX_COLS; col++) {
            sum += matrix[row * MATRIX_COLS + col] * vector[col];
        }
        result[row] = sum;
    }
}

int main() {
    struct dpu_set_t dpu_set;
    
    // Allocate host memory
    uint32_t *matrix = malloc(MATRIX_SIZE * sizeof(uint32_t));
    uint32_t *input_vector = malloc(MATRIX_COLS * sizeof(uint32_t));
    uint32_t *dpu_result = malloc(MATRIX_ROWS * sizeof(uint32_t));
    uint32_t *cpu_result = malloc(MATRIX_ROWS * sizeof(uint32_t));
    
    printf("=== Exercise 4: Matrix Operations (Challenge) ===\n");
    
    // Initialize test data
    printf("Initializing %d×%d matrix and %d-element vector...\n", 
           MATRIX_ROWS, MATRIX_COLS, MATRIX_COLS);
    
    // Create predictable test pattern
    srand(42);
    for (uint32_t i = 0; i < MATRIX_SIZE; i++) {
        matrix[i] = (i % 100) + 1;  // Values 1-100
    }
    
    for (uint32_t i = 0; i < MATRIX_COLS; i++) {
        input_vector[i] = i + 1;    // Values 1-64
    }
    
    printf("Matrix: %d elements, Vector: %d elements\n", MATRIX_SIZE, MATRIX_COLS);
    
    // Compute reference result on CPU
    printf("Computing reference result on CPU...\n");
    clock_t start_time = clock();
    cpu_matrix_vector_multiply(matrix, input_vector, cpu_result);
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;
    
    // TODO: Allocate DPU
    /* YOUR CODE HERE */
    
    // TODO: Load DPU program
    /* YOUR CODE HERE */
    
    // TODO: Transfer data to DPU
    printf("\nTransferring data to DPU...\n");
    printf("- Matrix: %lu bytes\n", MATRIX_SIZE * sizeof(uint32_t));
    printf("- Vector: %lu bytes\n", MATRIX_COLS * sizeof(uint32_t));
    
    // Transfer matrix
    /* YOUR CODE HERE */
    
    // Transfer input vector
    /* YOUR CODE HERE */
    
    // TODO: Launch matrix-vector multiplication
    printf("\nExecuting matrix-vector multiplication...\n");
    printf("Using %d tasklets, ~%d rows per tasklet\n", 
           11, (MATRIX_ROWS + 10) / 11);
    
    start_time = clock();
    /* YOUR CODE HERE */
    end_time = clock();
    double dpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;
    
    // TODO: Retrieve results from DPU
    printf("Retrieving results...\n");
    /* YOUR CODE HERE */
    
    // Verify results
    printf("\nVerification (first 5 elements):\n");
    uint32_t errors = 0;
    for (uint32_t i = 0; i < 5; i++) {
        if (dpu_result[i] == cpu_result[i]) {
            printf("Row %u: y[%u] = %u ✓\n", i, i, dpu_result[i]);
        } else {
            printf("Row %u: y[%u] = %u ✗ (expected %u)\n", 
                   i, i, dpu_result[i], cpu_result[i]);
            errors++;
        }
    }
    
    // Check all results
    for (uint32_t i = 0; i < MATRIX_ROWS; i++) {
        if (dpu_result[i] != cpu_result[i]) {
            errors++;
        }
    }
    
    if (errors == 0) {
        printf("\nMatrix-vector multiplication completed successfully!\n");
        printf("All %d results verified correct!\n", MATRIX_ROWS);
        
        // Performance analysis
        printf("\nPerformance Analysis:\n");
        printf("- Matrix operations: %d multiplications\n", MATRIX_SIZE);
        printf("- Vector reuse factor: %d× (each vector element used %d times)\n", 
               MATRIX_ROWS, MATRIX_ROWS);
        printf("- CPU execution time: %.2f ms\n", cpu_time);
        printf("- DPU execution time: %.2f ms\n", dpu_time);
        printf("- Memory efficiency: High (vector cached in WRAM)\n");
        
        if (dpu_time > 0) {
            printf("- Speedup: %.2fx\n", cpu_time / dpu_time);
        }
        
    } else {
        printf("\nFound %u errors in computation!\n", errors);
        
        // Show first few mismatches for debugging
        printf("\nFirst mismatches:\n");
        uint32_t shown = 0;
        for (uint32_t i = 0; i < MATRIX_ROWS && shown < 5; i++) {
            if (dpu_result[i] != cpu_result[i]) {
                printf("Row %u: DPU=%u, CPU=%u, diff=%d\n", 
                       i, dpu_result[i], cpu_result[i], 
                       (int)dpu_result[i] - (int)cpu_result[i]);
                shown++;
            }
        }
    }
    
    // TODO: Free DPU
    /* YOUR CODE HERE */
    
    // Free host memory
    free(matrix);
    free(input_vector);
    free(dpu_result);
    free(cpu_result);
    
    return 0;
}

/*
 * HINTS:
 * - Transfer matrix to "matrix" variable in DPU
 * - Transfer vector to "input_vector" variable in DPU
 * - Retrieve results from "result_vector" variable in DPU
 * - Use same patterns as previous exercises
 * - The DPU program handles the optimization
 * 
 * EXPECTED COMPUTATION:
 * Each result[i] = sum of (matrix[i][j] * vector[j]) for j=0 to 63
 * 
 * With the test pattern:
 * - Matrix elements: values 1-100 repeating
 * - Vector elements: values 1-64
 * - Results will be large numbers (thousands)
 */