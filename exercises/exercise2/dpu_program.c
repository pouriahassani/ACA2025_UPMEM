/**
 * Exercise 2: Array Processing - DPU Program
 * 
 * TODO: Complete this program to perform element-wise array addition
 * Each tasklet processes a portion of the arrays
 */

#include <stdint.h>
#include <defs.h>

#define ARRAY_SIZE 1024

// MRAM arrays - large data storage
__mram_noinit uint32_t array_a[ARRAY_SIZE];
__mram_noinit uint32_t array_b[ARRAY_SIZE];
__mram_noinit uint32_t result_array[ARRAY_SIZE];

int main() {
    // TODO: Get tasklet ID
    uint32_t tasklet_id = /* YOUR CODE HERE */;
    
    // TODO: Calculate work distribution
    // Each tasklet should process approximately ARRAY_SIZE/NR_TASKLETS elements
    uint32_t elements_per_tasklet = /* YOUR CODE HERE */;
    uint32_t start_index = /* YOUR CODE HERE */;
    uint32_t end_index = /* YOUR CODE HERE */;
    
    // Handle the last tasklet (may process fewer elements due to remainder)
    if (tasklet_id == NR_TASKLETS - 1) {
        end_index = ARRAY_SIZE;
    }
    
    // TODO: Process assigned array elements
    // Perform element-wise addition: result[i] = array_a[i] + array_b[i]
    for (uint32_t i = start_index; i < end_index; i++) {
        /* YOUR CODE HERE */
    }
    
    return 0;
}

/*
 * HINTS:
 * - elements_per_tasklet = ARRAY_SIZE / NR_TASKLETS
 * - start_index = tasklet_id * elements_per_tasklet  
 * - end_index = start_index + elements_per_tasklet
 * - MRAM arrays can be accessed directly like regular arrays
 * - Each tasklet works on a different portion of the arrays
 * 
 * WORK DISTRIBUTION EXAMPLE:
 * - Tasklet 0: elements 0-92 (93 elements)
 * - Tasklet 1: elements 93-185 (93 elements)
 * - ...
 * - Tasklet 10: elements 930-1023 (94 elements - handles remainder)
 */