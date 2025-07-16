# Exercise 3: Parallel Reduction

## Objective
Implement a parallel sum reduction algorithm using tasklet synchronization and shared memory.

## Background
Reduction operations (sum, max, min) are fundamental in parallel computing. This exercise teaches advanced concepts:
- **Inter-tasklet communication** using shared WRAM
- **Synchronization barriers** to coordinate tasklets
- **Parallel reduction trees** for efficient computation
- **Memory hierarchy optimization** (WRAM vs MRAM)

## Algorithm Overview
```
Input Array: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, ..., N]

Step 1: Each tasklet sums its portion
Tasklet 0: sum(elements 0-93)    → partial_sum_0
Tasklet 1: sum(elements 94-187)  → partial_sum_1
...
Tasklet 10: sum(elements 930-1023) → partial_sum_10

Step 2: Combine partial sums (tree reduction)
Level 1: partial_sum_0 + partial_sum_1 → temp_0
         partial_sum_2 + partial_sum_3 → temp_1
         ...
Level 2: temp_0 + temp_1 → temp_0
         ...
Final:   Single total sum
```

## Your Task

### Part A: Partial Sum Computation
Each tasklet computes the sum of its assigned array elements.

### Part B: Tree Reduction
Implement a parallel reduction tree to combine partial sums efficiently.

### Part C: Synchronization
Use barriers to ensure tasklets coordinate properly during reduction.

## Technical Challenges
- **Work distribution:** Handle array sizes not evenly divisible by 11
- **Shared memory:** Use WRAM for fast inter-tasklet communication  
- **Synchronization:** Coordinate tasklets during reduction phases
- **Tree reduction:** Implement efficient logarithmic reduction

## Files Provided
- `dpu_program.c` - DPU program template
- `host_program.c` - Host program template
- `Makefile` - Build script

## Expected Output
```
=== Exercise 3: Parallel Reduction ===
Creating test array with 2048 elements...
Expected sum: 2096128 (computed on CPU)

Transferring data to DPU...
Executing parallel reduction on DPU...
Retrieving result...

DPU Result: 2096128
CPU Result: 2096128
✓ Parallel reduction successful!

Performance Analysis:
- Array size: 2048 elements
- Tasklets used: 11
- Elements per tasklet: ~186
- Reduction levels: 4
```

## Key Learning Points
- **Barrier synchronization:** `barrier_wait(&my_barrier)`
- **Shared WRAM arrays:** Fast tasklet communication
- **Tree reduction pattern:** Efficient parallel algorithm
- **Work-sharing strategies:** Load balancing among tasklets

## Time Limit
20 minutes

## Compilation
```bash
make run
```

This is a challenging exercise that demonstrates real parallel programming!