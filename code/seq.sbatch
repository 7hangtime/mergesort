#!/bin/bash
#SBATCH --job-name=mergesort_seq
#SBATCH --output=mergesort_seq_%j.out
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --time=04:00:00
#SBATCH --partition=Centaurus

# Build sequential version
make -f Makefile_seq

echo "======================================"
echo " SLURM job information"
echo " Job ID: $SLURM_JOB_ID"
echo " Job Name: $SLURM_JOB_NAME"
echo " Node: $SLURM_NODELIST"
echo " CPUs: $SLURM_CPUS_PER_TASK"
echo "======================================"
echo ""

# Test size (modify if needed)
N=10000000

# Sequential run (no threads argument)
{ time ./mergesort_seq ${N} > seq_n${N}.out; } 2>&1

rm mergesort_seq
