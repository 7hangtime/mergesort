To run the code, Use Centaurus. There are two separate sbatch for the sequential, and for the parallel. Edit the sbatch with what you want. The amount of data to be used in the sort can be changed under the #Problem size comment by changing the vaiable N.



 When changing threads, also change the CPUs on the "--cpus-per-task" if you're using more than one thread.
 
 The time output is inside of the .out file thats returned when sbatch completes a job.
