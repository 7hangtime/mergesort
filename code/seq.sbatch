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

# Problem size
N=50000

echo "sequential run"
{ time ./mergesort_seq ${N}; } 2>&1

rm mergesort_seq
