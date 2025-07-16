# UPMEM Lab Exercises - Student Guide

## Welcome to Processing-In-Memory Programming!

In this lab, you'll learn to program UPMEM's Data Processing Units (DPUs) - processors embedded directly inside memory chips. You'll start with simple examples and progress to parallel algorithms.

---

## Exercise Overview

### Exercise 1: First DPU Program (Warm-up)
**File:** `exercise1/`  
**Goal:** Write your first DPU program and understand tasklets  
**Time:** 10 minutes

### Exercise 2: Array Processing 
**File:** `exercise2/`  
**Goal:** Implement element-wise array operations with host-DPU communication  
**Time:** 15 minutes

### Exercise 3: Parallel Reduction
**File:** `exercise3/`  
**Goal:** Implement parallel sum using multiple tasklets  
**Time:** 20 minutes

### Exercise 4: Matrix Operations (Challenge)
**File:** `exercise4/`  
**Goal:** Advanced memory management and 2D data processing  
**Time:** 15+ minutes

---

## 🚀 Quick Start: Compilation and Execution

### Step 1: Navigate to Exercise Directory
```bash
cd exercises/exercise1/  # Start with exercise 1
```

### Step 2: Compile Your Programs

**Compile the DPU program:**
```bash
dpu-upmem-dpurte-clang -DNR_TASKLETS=16 -o dpu_program dpu_program.c
```

**Compile the host program:**
```bash
gcc $(dpu-pkg-config --cflags --libs dpu) host_program.c -o host_program
```

### Step 3: Run Your Program
```bash
./host_program
```

The host program will automatically:
1. Load your DPU program
2. Send data to the DPUs
3. Execute the DPU program 
4. Retrieve and display results

---

## 🔧 Environment Setup Verification

Before starting, verify your environment:

```bash
# Check if UPMEM tools are available
which dpu-upmem-dpurte-clang
which dpu-pkg-config

# Check UPMEM version
dpu-pkg-config --version

# Verify simulator mode (should show "1" if no hardware)
echo $DPU_FALLBACK_TO_SIMULATOR
```

If any command fails, ask your instructor for help with environment setup.

---

## 📚 UPMEM Programming APIs

### DPU-Side Programming (dpu_program.c)

#### Core System Functions
```c
#include <defs.h>

// Get current tasklet ID (0 to 15, but we use 0-10 for NR_TASKLETS=11)
uint32_t tasklet_id = me();

// Total number of tasklets (set via -DNR_TASKLETS=16 compile flag)
NR_TASKLETS  // Compile-time constant
```

#### Memory Management
```c
#include <mram.h>

// MRAM memory declarations (64MB per DPU, slower)
__mram_noinit uint32_t large_array[1000000];  // Stored in MRAM
__mram_noinit uint32_t input_data[SIZE];      // Input from host

// WRAM memory declarations (64KB per DPU, faster)
__dma_aligned uint32_t cache_buffer[256];     // WRAM buffer for caching
uint32_t local_variable;                      // Regular WRAM variable

// Host-accessible variables (accessed by host via dpu_copy_to/from)
__host uint32_t shared_result;                // Single value from/to host
__host uint32_t results_array[NR_TASKLETS];   // Array shared with host

// Memory transfer functions (MRAM ↔ WRAM)
mram_read(source_mram_ptr, dest_wram_ptr, num_bytes);   // MRAM → WRAM
mram_write(source_wram_ptr, dest_mram_ptr, num_bytes);  // WRAM → MRAM
```

**⚠️ MRAM Alignment Rules:**
- All MRAM transfers must be **8-byte aligned** in both size and offset
- Use `__dma_aligned` for WRAM buffers that will be used with MRAM
- Minimum transfer size: 8 bytes

#### Synchronization
```c
#include <barrier.h>

// Declare a barrier for all tasklets
BARRIER_INIT(my_barrier, NR_TASKLETS);

// Wait for all tasklets to reach this point
barrier_wait(&my_barrier);
```

#### Standard C Libraries
```c
#include <stdint.h>    // For uint32_t, uint64_t types
#include <stdio.h>     // For printf (use sparingly in DPU code)
```

### Host-Side Programming (host_program.c)

#### Core DPU Management
```c
#include <dpu.h>

struct dpu_set_t dpu_set, dpu;  // DPU handles

// 1. Allocate DPUs
DPU_ASSERT(dpu_alloc(num_dpus, NULL, &dpu_set));

// 2. Load DPU program binary
DPU_ASSERT(dpu_load(dpu_set, "dpu_program", NULL));

// 3. Transfer data TO DPUs
DPU_ASSERT(dpu_copy_to(dpu_set, "variable_name", 0, host_data, data_size));

// 4. Execute DPU program
DPU_ASSERT(dpu_launch(dpu_set, DPU_SYNCHRONOUS));

// 5. Transfer data FROM DPUs  
DPU_FOREACH(dpu_set, dpu) {
    DPU_ASSERT(dpu_copy_from(dpu, "variable_name", 0, host_buffer, buffer_size));
}

// 6. Free DPUs
DPU_ASSERT(dpu_free(dpu_set));
```

#### Data Transfer Details
```c
// Copy data to ALL DPUs in the set
dpu_copy_to(dpu_set, "dpu_variable_name", offset, &host_data, sizeof(host_data));

// Copy data from EACH DPU individually
DPU_FOREACH(dpu_set, dpu) {
    dpu_copy_from(dpu, "dpu_variable_name", offset, &host_buffer, sizeof(host_buffer));
}
```

#### Error Handling
```c
#include <dpu_log.h>

// Always wrap DPU API calls with DPU_ASSERT
DPU_ASSERT(dpu_function_call(...));  // Exits program if error occurs

// For custom error handling
dpu_error_t err = dpu_function_call(...);
if (err != DPU_OK) {
    fprintf(stderr, "DPU error: %s\n", dpu_error_to_string(err));
    return -1;
}
```

---

## 💡 Programming Patterns and Best Practices

### Pattern 1: Work Distribution Among Tasklets
```c
// DPU side - distribute array processing
uint32_t tasklet_id = me();
uint32_t elements_per_tasklet = ARRAY_SIZE / NR_TASKLETS;
uint32_t start_index = tasklet_id * elements_per_tasklet;
uint32_t end_index = start_index + elements_per_tasklet;

// Handle remainder for last tasklet
if (tasklet_id == NR_TASKLETS - 1) {
    end_index = ARRAY_SIZE;
}

// Process assigned range
for (uint32_t i = start_index; i < end_index; i++) {
    result[i] = process(input[i]);
}
```

### Pattern 2: Parallel Reduction
```c
// Phase 1: Each tasklet computes partial result
uint32_t partial_result = compute_partial_sum(my_data_range);
partial_sums[tasklet_id] = partial_result;

barrier_wait(&sync_barrier);

// Phase 2: Tree reduction (tasklet 0 combines all results)
if (tasklet_id == 0) {
    uint32_t total = 0;
    for (uint32_t i = 0; i < NR_TASKLETS; i++) {
        total += partial_sums[i];
    }
    final_result = total;
}
```

### Pattern 3: MRAM Caching for Performance
```c
// Cache frequently accessed data in WRAM
__dma_aligned uint32_t wram_cache[CACHE_SIZE];

// Load data from MRAM to WRAM cache
mram_read(&mram_data[offset], wram_cache, CACHE_SIZE * sizeof(uint32_t));

// Process data in fast WRAM
for (uint32_t i = 0; i < CACHE_SIZE; i++) {
    wram_cache[i] = process(wram_cache[i]);
}

// Write results back to MRAM
mram_write(wram_cache, &mram_results[offset], CACHE_SIZE * sizeof(uint32_t));
```

---

## 🔍 Memory Architecture Understanding

### UPMEM Memory Hierarchy
```
Host Memory (DDR4)
       ↕ dpu_copy_to/from() - slow, large transfers
DPU MRAM (64MB per DPU)
       ↕ mram_read/write() - medium speed, 8-byte aligned
DPU WRAM (64KB per DPU)
       ↕ Direct access - fast, where computation happens
DPU Registers
```

### Memory Usage Guidelines
- **Host Variables**: Use for results that need to be read by host
- **MRAM Variables**: Use for large datasets (input/output arrays)
- **WRAM Variables**: Use for temporary computations and caching
- **Local Variables**: Stored in WRAM, fastest access

---

## 🛠️ Compilation Flags and Options

### Essential Compiler Flags
```bash
# Set number of tasklets (required!)
-DNR_TASKLETS=16

# Optimization levels
-O0    # No optimization (default, good for debugging)
-O2    # Standard optimization
-O3    # Aggressive optimization

# Include paths (usually automatic)
-I/path/to/upmem/include/syslib
-I/path/to/upmem/include/stdlib
```

### Host Compilation with pkg-config
```bash
# Get compilation flags
dpu-pkg-config --cflags    # Returns: -I/usr/include/dpu

# Get linking flags  
dpu-pkg-config --libs      # Returns: -ldpu

# Combined compilation
gcc $(dpu-pkg-config --cflags --libs dpu) host_program.c -o host_program
```

---

## ⚡ Performance Tips

### DO:
- ✅ Cache frequently accessed MRAM data in WRAM
- ✅ Use barriers only when necessary for synchronization
- ✅ Distribute work evenly among tasklets
- ✅ Align MRAM transfers to 8-byte boundaries
- ✅ Minimize data transfers between host and DPU

### DON'T:
- ❌ Access MRAM directly in tight loops (use WRAM caching)
- ❌ Use printf() extensively in DPU code (slows execution)
- ❌ Forget to handle remainder elements in work distribution
- ❌ Mix up MRAM and WRAM variable declarations
- ❌ Skip error checking with DPU_ASSERT()

---

## 🐛 Common Issues and Solutions

### Compilation Errors
```bash
# Error: "dpu-upmem-dpurte-clang: command not found"
# Solution: Source the UPMEM environment
source /path/to/upmem/upmem_env.sh

# Error: "NR_TASKLETS not defined"  
# Solution: Add compilation flag
dpu-upmem-dpurte-clang -DNR_TASKLETS=16 ...

# Error: "undefined reference to dpu_alloc"
# Solution: Add DPU libraries to host compilation
gcc $(dpu-pkg-config --cflags --libs dpu) host_program.c -o host_program
```

### Runtime Errors
```bash
# Error: "DPU allocation failed"
# Solution: Make sure simulator mode is enabled
export DPU_FALLBACK_TO_SIMULATOR=1

# Error: "invalid mram access (alignment)"
# Solution: Ensure 8-byte alignment for MRAM operations
# Use __dma_aligned for WRAM buffers

# Error: "symbol not found"  
# Solution: Check variable names match exactly between dpu_copy calls and DPU variables
```

### Performance Issues
```bash
# Issue: Program runs slowly
# Check: Are you accessing MRAM directly in loops? Use WRAM caching
# Check: Are you using too many barriers? Minimize synchronization points

# Issue: Incorrect results
# Check: Work distribution - is the last tasklet handling remainders?
# Check: Race conditions - are you using barriers where needed?
```

---

## 📋 Exercise Progression

### Start Here: Exercise 1
- Learn basic tasklet programming
- Understand me() function and NR_TASKLETS
- Practice simple computations and result storage

### Then: Exercise 2  
- Learn host-DPU data communication
- Practice MRAM variable declarations
- Understand work distribution patterns

### Next: Exercise 3
- Master barrier synchronization
- Implement parallel reduction algorithms  
- Learn efficient parallel programming patterns

### Finally: Exercise 4
- Advanced MRAM-WRAM management
- Optimize memory access patterns
- Handle complex 2D data structures

---

## 🎯 Success Criteria

For each exercise, your program should:
1. **Compile without errors** using the provided compilation commands
2. **Run successfully** and produce expected output  
3. **Handle edge cases** (like remainder elements in work distribution)
4. **Use appropriate memory types** (MRAM for data, WRAM for computation)
5. **Synchronize properly** using barriers where needed

---

## 🆘 Getting Help

1. **Read error messages carefully** - they often point to the exact problem
2. **Check variable names** - they must match exactly between host and DPU code
3. **Verify compilation flags** - especially -DNR_TASKLETS=16
4. **Test incrementally** - start with simple functionality, then add complexity
5. **Ask for help** - your instructor is here to help you succeed!

---

**Ready to explore Processing-In-Memory computing?**  
**Start with Exercise 1 and work your way through!**

**Remember**: The goal is to understand how to move computation closer to data, reducing the memory bottleneck that limits traditional computing systems.