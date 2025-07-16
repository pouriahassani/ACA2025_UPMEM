# Exercise 1: First DPU Program

## Objective
Write your first DPU program to understand the basic programming model and tasklet parallelism.

## Background
Each DPU has 11 tasklets (lightweight threads) that execute the same program in parallel. Each tasklet has a unique ID (0-10) and can perform different computations based on this ID.

## Your Task

### Part A: Basic Tasklet Program
Complete the DPU program that:
1. Gets the current tasklet ID
2. Performs a simple computation
3. Stores the result in a shared array

### Part B: Host Controller
Write a host program that:
1. Allocates a single DPU
2. Loads and executes your DPU program
3. Retrieves and displays results

## Files Provided
- `dpu_program.c` - DPU program template (you complete this)
- `host_program.c` - Host program template (you complete this)
- `Makefile` - Build script

## Expected Output
```
Tasklet 0 computed: 0
Tasklet 1 computed: 10
Tasklet 2 computed: 20
...
Tasklet 10 computed: 100
```

## Hints
- Use `me()` to get tasklet ID
- Use `DPU_ASSERT()` for error checking
- Remember to free DPUs after use

## Time Limit
10 minutes

## Compilation
```bash
make
./host_program
```

Good luck! Ask for help if you get stuck.