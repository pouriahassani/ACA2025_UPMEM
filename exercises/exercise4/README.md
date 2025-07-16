# Exercise 4: Matrix Operations (Challenge)

## Objective
Implement matrix-vector multiplication with advanced memory management and optimization techniques.

## Background
This challenging exercise combines all previous concepts:
- **2D data structures** and matrix operations
- **Advanced memory patterns** (WRAM buffering, MRAM streaming)
- **Load balancing** across tasklets
- **Performance optimization** techniques

## Algorithm: Matrix-Vector Multiplication
```
Given:
- Matrix A: M×N (stored row-major)
- Vector x: N×1  
- Result vector y: M×1

Compute: y[i] = Σ(A[i][j] * x[j]) for j=0 to N-1

Example (3×4 matrix):
A = [[1, 2, 3, 4],     x = [1]     y = [1*1 + 2*2 + 3*3 + 4*4] = [30]
     [5, 6, 7, 8],          [2]         [5*1 + 6*2 + 7*3 + 8*4]   [70]  
     [9,10,11,12]]          [3]         [9*1 +10*2 +11*3 +12*4]   [110]
                            [4]
```

## Your Task

### Part A: Memory Layout Design
Design efficient memory layout for matrix storage and access patterns.

### Part B: Work Distribution Strategy  
Implement smart work distribution - each tasklet processes several matrix rows.

### Part C: WRAM Buffering Optimization
Use WRAM as cache for frequently accessed vector elements.

### Part D: Performance Analysis
Compare different optimization strategies.

## Technical Specifications
- **Matrix Size:** 128×64 (8,192 elements total)
- **Vector Size:** 64 elements
- **Result Size:** 128 elements
- **Memory Strategy:** MRAM for matrix, WRAM for vector caching
- **Work Distribution:** ~12 rows per tasklet

## Advanced Concepts
1. **Memory Coalescing:** Efficient MRAM access patterns
2. **Data Reuse:** Cache frequently accessed data in WRAM
3. **Load Balancing:** Handle uneven work distribution
4. **Pipeline Optimization:** Overlap computation and memory access

## Files Provided
- `dpu_program.c` - DPU program template
- `host_program.c` - Host program template  
- `Makefile` - Build script

## Expected Output
```
=== Exercise 4: Matrix Operations (Challenge) ===
Initializing 128×64 matrix and 64-element vector...
Matrix: 8192 elements, Vector: 64 elements

Transferring data to DPU...
- Matrix: 32768 bytes
- Vector: 256 bytes

Executing matrix-vector multiplication...
Using 11 tasklets, ~12 rows per tasklet

Retrieving results...

Verification (first 5 elements):
Row 0: y[0] = 83200 ✓
Row 1: y[1] = 86720 ✓  
Row 2: y[2] = 90240 ✓
Row 3: y[3] = 93760 ✓
Row 4: y[4] = 97280 ✓

Matrix-vector multiplication completed successfully!
All 128 results verified correct!

Performance Analysis:
- Matrix operations: 8192 multiplications
- Vector reuse factor: 128× (each vector element used 128 times)
- Memory efficiency: High (vector cached in WRAM)
```

## Optimization Strategies to Implement
1. **Vector Caching:** Load entire vector into WRAM once
2. **Row-wise Processing:** Process complete matrix rows sequentially  
3. **Accumulator Reuse:** Minimize MRAM writes
4. **Memory Alignment:** Ensure efficient transfers

## Time Limit
20+ minutes (This is the most challenging exercise!)

## Compilation
```bash
make run
```

## Tips for Success
- Start with basic functionality, then optimize
- Use WRAM strategically for frequently accessed data
- Consider memory access patterns carefully
- Test with smaller matrices first if needed

This exercise demonstrates real-world HPC programming techniques!