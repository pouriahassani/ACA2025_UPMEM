/**
 * Exercise 1: First DPU Program
 * 
 * TODO: Complete this DPU program to:
 * 1. Get the current tasklet ID using me()
 * 2. Compute result = tasklet_id * 10
 * 3. Store the result in the results array
 */

#include <stdint.h>
#include <defs.h>

// Shared array to store results from all tasklets
// Note: This array is shared among all tasklets in the DPU
__host uint32_t results[NR_TASKLETS];

int main() {
    // TODO: Get the current tasklet ID
    uint32_t tasklet_id = /* YOUR CODE HERE */;
    
    // TODO: Perform computation (multiply tasklet_id by 10)
    uint32_t computation_result = /* YOUR CODE HERE */;
    
    // TODO: Store result in the shared array
    /* YOUR CODE HERE */
    
    return 0;
}

/* 
 * HINTS:
 * - me() returns the current tasklet ID (0 to 10)
 * - results[tasklet_id] = value stores value at index tasklet_id
 * - NR_TASKLETS is a predefined constant (11)
 * - __host qualifier makes variable accessible from host
 */