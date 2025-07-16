/**
 * Exercise 3: Parallel Reduction - DPU Program
 * 
 * TODO: Implement parallel sum reduction using:
 * 1. Each tasklet computes partial sum of its elements
 * 2. Tree reduction to combine partial sums
 * 3. Barrier synchronization between phases
 */

#include <stdint.h>
#include <defs.h>
#include <barrier.h>

#define ARRAY_SIZE 2048

// Input array in MRAM (large, slower access)
__mram_noinit uint32_t input_array[ARRAY_SIZE];

// Shared WRAM arrays for fast inter-tasklet communication
__dma_aligned uint32_t partial_sums[NR_TASKLETS];   // Each tasklet's partial sum
__dma_aligned uint32_t reduction_buffer[NR_TASKLETS]; // Temporary buffer for tree reduction

// Final result (accessible by host)
__host uint32_t final_sum;

// Barrier for tasklet synchronization
BARRIER_INIT(reduction_barrier, NR_TASKLETS);

int main() {
    uint32_t tasklet_id = me();
    
    // Phase 1: Compute partial sums
    // TODO: Calculate this tasklet's work range
    uint32_t elements_per_tasklet = /* YOUR CODE HERE */;
    uint32_t start_index = /* YOUR CODE HERE */;
    uint32_t end_index = /* YOUR CODE HERE */;
    
    // Handle remainder elements in last tasklet
    if (tasklet_id == NR_TASKLETS - 1) {
        end_index = ARRAY_SIZE;
    }
    
    // TODO: Compute partial sum for this tasklet's range
    uint32_t my_partial_sum = 0;
    for (uint32_t i = start_index; i < end_index; i++) {
        /* YOUR CODE HERE */
    }
    
    // Store partial sum in shared array
    partial_sums[tasklet_id] = my_partial_sum;
    
    // TODO: Wait for all tasklets to complete Phase 1
    /* YOUR CODE HERE */
    
    // Phase 2: Tree reduction
    // Copy partial sums to reduction buffer (only tasklet 0 needs to do this)
    if (tasklet_id == 0) {
        for (uint32_t i = 0; i < NR_TASKLETS; i++) {
            reduction_buffer[i] = partial_sums[i];
        }
    }
    
    // TODO: Wait for buffer initialization
    /* YOUR CODE HERE */
    
    // Tree reduction: combine sums level by level
    uint32_t active_tasklets = NR_TASKLETS;
    
    while (active_tasklets > 1) {
        // TODO: Calculate how many tasklets participate in this level
        uint32_t tasklets_this_level = /* YOUR CODE HERE */;
        
        // Only some tasklets participate in each level
        if (tasklet_id < tasklets_this_level) {
            // TODO: Calculate indices for tree reduction
            uint32_t partner_index = /* YOUR CODE HERE */;
            
            // TODO: Add partner's value to my value (if partner exists)
            if (partner_index < active_tasklets) {
                /* YOUR CODE HERE */
            }
        }
        
        // TODO: Wait for all active tasklets to complete this level
        /* YOUR CODE HERE */
        
        // Update number of active tasklets for next level
        active_tasklets = tasklets_this_level;
    }
    
    // Tasklet 0 stores the final result
    if (tasklet_id == 0) {
        final_sum = reduction_buffer[0];
    }
    
    return 0;
}

/*
 * HINTS FOR TREE REDUCTION:
 * 
 * Level 0: 11 tasklets → 6 tasklets
 * - Tasklet 0 adds value from tasklet 1
 * - Tasklet 2 adds value from tasklet 3  
 * - Tasklet 4 adds value from tasklet 5
 * - ...
 * - Tasklet 10 has no partner (odd number)
 * 
 * Level 1: 6 tasklets → 3 tasklets  
 * - Tasklet 0 adds value from tasklet 2
 * - Tasklet 4 adds value from tasklet 6
 * - Tasklet 8 adds value from tasklet 10
 * 
 * Level 2: 3 tasklets → 2 tasklets
 * - Tasklet 0 adds value from tasklet 4
 * - Tasklet 8 has no partner
 * 
 * Level 3: 2 tasklets → 1 tasklet
 * - Tasklet 0 adds value from tasklet 8
 * 
 * FORMULAS:
 * - tasklets_this_level = (active_tasklets + 1) / 2
 * - partner_index = tasklet_id + step_size
 * - step_size = active_tasklets / tasklets_this_level
 * 
 * SIMPLIFIED APPROACH:
 * - partner_index = tasklet_id + (active_tasklets / 2)
 * - if partner_index exists: my_value += partner_value
 * - active_tasklets = (active_tasklets + 1) / 2
 */