# Exercise 2: Array Processing

## Objective
Implement element-wise array operations with proper host-DPU communication and memory management.

## Background
Real-world DPU programs process data arrays. This exercise teaches you how to:
- Transfer data from host to DPU memory (MRAM)
- Process arrays in parallel using tasklets
- Retrieve results back to the host

## Your Task

### Part A: DPU Array Addition
Complete the DPU program that:
1. Receives two input arrays from the host
2. Distributes work among tasklets (each processes different elements)
3. Computes element-wise addition: `result[i] = array_a[i] + array_b[i]`
4. Stores results in output array

### Part B: Host Data Management
Complete the host program that:
1. Creates input arrays with test data
2. Transfers arrays to DPU memory
3. Launches DPU computation
4. Retrieves and verifies results

## Technical Details
- **Array Size:** 1024 elements (each uint32_t)
- **Work Distribution:** Each tasklet processes ~93 elements (1024/11)
- **Memory:** Use MRAM for array storage
- **Synchronization:** Not needed (independent element operations)

## Files Provided
- `dpu_program.c` - DPU program template
- `host_program.c` - Host program template
- `Makefile` - Build script

## Expected Output
```
=== Exercise 2: Array Processing ===
Initializing arrays...
Arrays created: A[0..1023], B[0..1023]
Transferring data to DPU...
Executing array addition on DPU...
Retrieving results...

Verification (first 10 elements):
A[0] + B[0] = 0 + 1000 = 1000 ✓
A[1] + B[1] = 1 + 1001 = 1002 ✓
...
A[9] + B[9] = 9 + 1009 = 1018 ✓

Array addition completed successfully!
All 1024 elements verified correct!
```

## Key Concepts
- **MRAM arrays:** Large data storage on DPU
- **Work distribution:** Dividing array among tasklets
- **Memory transfers:** Host ↔ DPU communication
- **Data validation:** Checking computational correctness

## Time Limit
15 minutes

## Compilation
```bash
make run
```

This exercise is foundational for all parallel array processing!