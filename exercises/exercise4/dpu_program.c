/**
 * Exercise 4: Matrix Operations - DPU Program
 * 
 * TODO: Implement optimized matrix-vector multiplication
 * This is the most challenging exercise combining all concepts!
 */

#include <stdint.h>
#include <defs.h>
#include <mram.h>

#define MATRIX_ROWS 128
#define MATRIX_COLS 64
#define MATRIX_SIZE (MATRIX_ROWS * MATRIX_COLS)

// MRAM storage for large data
__mram_noinit uint32_t matrix[MATRIX_SIZE];     // 128×64 matrix (row-major)
__mram_noinit uint32_t result_vector[MATRIX_ROWS]; // Output vector

// WRAM cache for frequently accessed data (fast access)
__dma_aligned uint32_t vector_cache[MATRIX_COLS];   // Cache input vector in WRAM
__dma_aligned uint32_t row_buffer[MATRIX_COLS];     // Buffer for matrix row

// Input vector in MRAM  
__mram_noinit uint32_t input_vector[MATRIX_COLS];

int main() {
    uint32_t tasklet_id = me();
    
    // Phase 1: Load input vector into WRAM cache (only tasklet 0)
    if (tasklet_id == 0) {
        // TODO: Copy input vector from MRAM to WRAM cache
        // Use mram_read() for efficient transfer
        /* YOUR CODE HERE */
    }
    
    // Wait for vector caching to complete (if using barriers)
    // Note: Since only tasklet 0 writes and others read, 
    // we need to ensure the cache is ready
    
    // Phase 2: Calculate work distribution
    // TODO: Determine which matrix rows this tasklet will process
    uint32_t rows_per_tasklet = /* YOUR CODE HERE */;
    uint32_t start_row = /* YOUR CODE HERE */;
    uint32_t end_row = /* YOUR CODE HERE */;
    
    // Handle remainder rows in last tasklet
    if (tasklet_id == NR_TASKLETS - 1) {
        end_row = MATRIX_ROWS;
    }
    
    // Phase 3: Process assigned matrix rows
    for (uint32_t row = start_row; row < end_row; row++) {
        
        // TODO: Load current matrix row into WRAM buffer
        // Calculate MRAM address for this row
        uint32_t row_offset = /* YOUR CODE HERE */;
        /* YOUR CODE HERE - use mram_read() */
        
        // TODO: Compute dot product for this row
        uint32_t dot_product = 0;
        for (uint32_t col = 0; col < MATRIX_COLS; col++) {
            /* YOUR CODE HERE */
        }
        
        // TODO: Store result back to MRAM
        /* YOUR CODE HERE - use mram_write() */
    }
    
    return 0;
}

/*
 * OPTIMIZATION HINTS:
 * 
 * 1. MEMORY LAYOUT:
 *    - Matrix stored row-major: A[i][j] = matrix[i * MATRIX_COLS + j]
 *    - Row offset = row * MATRIX_COLS
 * 
 * 2. EFFICIENT MRAM ACCESS:
 *    - Use mram_read()/mram_write() for block transfers
 *    - Transfer entire rows at once: mram_read(src, dst, MATRIX_COLS * sizeof(uint32_t))
 *    - Source address: &matrix[row * MATRIX_COLS]
 * 
 * 3. WRAM CACHING STRATEGY:
 *    - Load input vector once into vector_cache[]
 *    - Load each matrix row into row_buffer[]
 *    - Perform computation using cached data
 * 
 * 4. DOT PRODUCT COMPUTATION:
 *    - dot_product += row_buffer[col] * vector_cache[col]
 * 
 * 5. WORK DISTRIBUTION:
 *    - rows_per_tasklet = MATRIX_ROWS / NR_TASKLETS
 *    - start_row = tasklet_id * rows_per_tasklet
 *    - end_row = start_row + rows_per_tasklet
 * 
 * 6. MEMORY ALIGNMENT:
 *    - MRAM transfers should be 8-byte aligned
 *    - Use __dma_aligned for WRAM buffers
 * 
 * EXAMPLE MEMORY LAYOUT:
 * Matrix (row-major):
 * Row 0: [A[0][0], A[0][1], ..., A[0][63]]
 * Row 1: [A[1][0], A[1][1], ..., A[1][63]]
 * ...
 * Row 127: [A[127][0], A[127][1], ..., A[127][63]]
 * 
 * Work Distribution Example:
 * - Tasklet 0: rows 0-11   (12 rows)
 * - Tasklet 1: rows 12-23  (12 rows)  
 * - ...
 * - Tasklet 10: rows 120-127 (8 rows - remainder)
 */