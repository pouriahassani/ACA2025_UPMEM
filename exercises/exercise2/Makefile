# Exercise 2 Makefile

# Compiler settings
DPU_CC = dpu-upmem-dpurte-clang
HOST_CC = gcc
DPU_CFLAGS = -O2 -g
HOST_CFLAGS = $(shell dpu-pkg-config --cflags --libs dpu) -O2 -g

# Default target
all: dpu_program host_program

# Build DPU program
dpu_program: dpu_program.c
	$(DPU_CC) $(DPU_CFLAGS) -o $@ $<

# Build host program  
host_program: host_program.c
	$(HOST_CC) $(HOST_CFLAGS) -o $@ $<

# Run the exercise
run: all
	./host_program

# Clean build artifacts
clean:
	rm -f dpu_program host_program

.PHONY: all run clean