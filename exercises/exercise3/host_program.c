/**
 * Exercise 3: Parallel Reduction - Host Program
 * 
 * TODO: Complete this program to test the parallel reduction
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dpu.h>

#define NR_DPUS 1
#define ARRAY_SIZE 2048

// CPU implementation for verification
uint32_t cpu_sum(uint32_t *array, uint32_t size) {
    uint32_t sum = 0;
    for (uint32_t i = 0; i < size; i++) {
        sum += array[i];
    }
    return sum;
}

int main() {
    struct dpu_set_t dpu_set;
    uint32_t *input_array = malloc(ARRAY_SIZE * sizeof(uint32_t));
    uint32_t dpu_result, cpu_result;
    
    printf("=== Exercise 3: Parallel Reduction ===\n");
    
    // Create test data
    printf("Creating test array with %d elements...\n", ARRAY_SIZE);
    srand(42); // Fixed seed for reproducible results
    for (uint32_t i = 0; i < ARRAY_SIZE; i++) {
        input_array[i] = i + 1; // Simple sequence: 1, 2, 3, ..., ARRAY_SIZE
    }
    
    // Compute expected result on CPU
    cpu_result = cpu_sum(input_array, ARRAY_SIZE);
    printf("Expected sum: %u (computed on CPU)\n", cpu_result);
    
    // TODO: Allocate DPU
    /* YOUR CODE HERE */
    
    // TODO: Load DPU program
    /* YOUR CODE HERE */
    
    // TODO: Transfer input array to DPU
    printf("\nTransferring data to DPU...\n");
    /* YOUR CODE HERE */
    
    // TODO: Launch parallel reduction on DPU
    printf("Executing parallel reduction on DPU...\n");
    /* YOUR CODE HERE */
    
    // TODO: Retrieve result from DPU
    printf("Retrieving result...\n");
    /* YOUR CODE HERE */
    
    // Display and verify results
    printf("\nResults:\n");
    printf("DPU Result: %u\n", dpu_result);
    printf("CPU Result: %u\n", cpu_result);
    
    if (dpu_result == cpu_result) {
        printf("✓ Parallel reduction successful!\n");
        
        // Performance analysis
        printf("\nPerformance Analysis:\n");
        printf("- Array size: %d elements\n", ARRAY_SIZE);
        printf("- Tasklets used: 11\n");
        printf("- Elements per tasklet: ~%d\n", ARRAY_SIZE / 11);
        
        // Calculate reduction tree levels
        int levels = 0;
        int active = 11;
        while (active > 1) {
            active = (active + 1) / 2;
            levels++;
        }
        printf("- Reduction levels: %d\n", levels);
        
    } else {
        printf("✗ Error: Results don't match!\n");
        printf("  Difference: %d\n", (int)dpu_result - (int)cpu_result);
    }
    
    // TODO: Free DPU
    /* YOUR CODE HERE */
    
    free(input_array);
    return 0;
}

/*
 * HINTS:
 * - Copy input array to "input_array" variable in DPU
 * - Retrieve result from "final_sum" variable in DPU  
 * - Use the same patterns as previous exercises
 * - The DPU program handles all the parallel computation
 * 
 * EXPECTED SUM CALCULATION:
 * For array [1, 2, 3, ..., N]:
 * Sum = N * (N + 1) / 2
 * For N = 2048: Sum = 2048 * 2049 / 2 = 2,096,128
 */