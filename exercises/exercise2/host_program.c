/**
 * Exercise 2: Array Processing - Host Program
 * 
 * TODO: Complete this program to:
 * 1. Create and initialize test arrays
 * 2. Transfer data to DPU
 * 3. Execute computation
 * 4. Retrieve and verify results
 */

#include <stdio.h>
#include <stdlib.h>
#include <dpu.h>

#define NR_DPUS 1
#define ARRAY_SIZE 1024

int main() {
    struct dpu_set_t dpu_set;
    
    // Host arrays
    uint32_t *array_a = malloc(ARRAY_SIZE * sizeof(uint32_t));
    uint32_t *array_b = malloc(ARRAY_SIZE * sizeof(uint32_t));
    uint32_t *result_array = malloc(ARRAY_SIZE * sizeof(uint32_t));
    
    printf("=== Exercise 2: Array Processing ===\n");
    
    // Initialize test data
    printf("Initializing arrays...\n");
    for (uint32_t i = 0; i < ARRAY_SIZE; i++) {
        array_a[i] = i;                    // A: [0, 1, 2, ..., 1023]
        array_b[i] = 1000 + i;            // B: [1000, 1001, 1002, ..., 2023]
    }
    printf("Arrays created: A[0..%d], B[0..%d]\n", ARRAY_SIZE-1, ARRAY_SIZE-1);
    
    // TODO: Allocate DPU
    /* YOUR CODE HERE */
    
    // TODO: Load DPU program
    /* YOUR CODE HERE */
    
    // TODO: Transfer input arrays to DPU
    printf("Transferring data to DPU...\n");
    // Copy array_a to DPU's "array_a" variable
    /* YOUR CODE HERE */
    // Copy array_b to DPU's "array_b" variable  
    /* YOUR CODE HERE */
    
    // TODO: Launch DPU computation
    printf("Executing array addition on DPU...\n");
    /* YOUR CODE HERE */
    
    // TODO: Retrieve results from DPU
    printf("Retrieving results...\n");
    /* YOUR CODE HERE */
    
    // Verify results
    printf("\nVerification (first 10 elements):\n");
    uint32_t errors = 0;
    for (uint32_t i = 0; i < 10; i++) {
        uint32_t expected = array_a[i] + array_b[i];
        if (result_array[i] == expected) {
            printf("A[%u] + B[%u] = %u + %u = %u ✓\n", 
                   i, i, array_a[i], array_b[i], result_array[i]);
        } else {
            printf("A[%u] + B[%u] = %u + %u = %u ✗ (got %u)\n", 
                   i, i, array_a[i], array_b[i], expected, result_array[i]);
            errors++;
        }
    }
    
    // Check all elements
    for (uint32_t i = 0; i < ARRAY_SIZE; i++) {
        if (result_array[i] != array_a[i] + array_b[i]) {
            errors++;
        }
    }
    
    if (errors == 0) {
        printf("\nArray addition completed successfully!\n");
        printf("All %d elements verified correct!\n", ARRAY_SIZE);
    } else {
        printf("\nFound %u errors in computation!\n", errors);
    }
    
    // TODO: Free DPU
    /* YOUR CODE HERE */
    
    // Free host memory
    free(array_a);
    free(array_b);
    free(result_array);
    
    return 0;
}

/*
 * HINTS:
 * - Use dpu_copy_to() to send data: dpu_copy_to(dpu_set, "variable_name", 0, host_array, size)
 * - Use dpu_copy_from() to get data: dpu_copy_from(dpu_set, "variable_name", 0, host_array, size)
 * - Size should be: ARRAY_SIZE * sizeof(uint32_t)
 * - Variable names must match those in the DPU program ("array_a", "array_b", "result_array")
 * - Always wrap DPU calls with DPU_ASSERT()
 */