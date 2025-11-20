#!/bin/bash
#SBATCH --job-name=mergesort_par
#SBATCH --output=mergesort_par_%j.out
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --time=04:00:00
#SBATCH --partition=Centaurus

# Build parallel version
make -f Makefile_par

echo "======================================"
echo " SLURM job information"
echo " Job ID: $SLURM_JOB_ID"
echo " Job Name: $SLURM_JOB_NAME"
echo " Node: $SLURM_NODELIST"
echo " CPUs: $SLURM_CPUS_PER_TASK"
echo "======================================"
echo ""

# Problem size
N=10000000

# 1 thread
{ time ./mergesort_par ${N} 1 > par_n${N}_t1.out; } 2>&1

# 2 threads
{ time ./mergesort_par ${N} 2 > par_n${N}_t2.out; } 2>&1

# 4 threads
{ time ./mergesort_par ${N} 4 > par_n${N}_t4.out; } 2>&1

# 8 threads
{ time ./mergesort_par ${N} 8 > par_n${N}_t8.out; } 2>&1

rm mergesort_par
