/**
 * Exercise 1: Host Program
 * 
 * TODO: Complete this host program to:
 * 1. Allocate a single DPU
 * 2. Load the DPU program
 * 3. Execute the program
 * 4. Retrieve and display results
 */

#include <stdio.h>
#include <dpu.h>

#define NR_DPUS 1
#define NR_TASKLETS 11

int main() {
    struct dpu_set_t dpu_set;
    uint32_t results[NR_TASKLETS];
    
    printf("=== Exercise 1: First DPU Program ===\n");
    
    // TODO: Allocate DPUs
    // Use dpu_alloc(count, profile, dpu_set_ptr)
    /* YOUR CODE HERE */
    
    // TODO: Load the DPU program
    // Use dpu_load(dpu_set, binary_path, NULL)
    /* YOUR CODE HERE */
    
    // TODO: Launch the DPU program
    // Use dpu_launch(dpu_set, DPU_SYNCHRONOUS)
    /* YOUR CODE HERE */
    
    // TODO: Copy results from DPU to host
    // Use dpu_copy_from(dpu_set, "results", 0, results, sizeof(results))
    /* YOUR CODE HERE */
    
    // Display results
    printf("\nResults from DPU:\n");
    for (int i = 0; i < NR_TASKLETS; i++) {
        printf("Tasklet %d computed: %u\n", i, results[i]);
    }
    
    // TODO: Free the DPUs
    // Use dpu_free(dpu_set)
    /* YOUR CODE HERE */
    
    printf("\nExercise 1 completed successfully!\n");
    return 0;
}

/*
 * HINTS:
 * - Always use DPU_ASSERT() around DPU function calls
 * - DPU_ASSERT(function_call) automatically checks for errors
 * - The binary name should be "dpu_program" (without .c extension)
 * - "results" refers to the __host variable name in the DPU program
 */