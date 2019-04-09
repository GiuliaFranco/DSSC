#!/bin/bash

for i in 2 4 10 16 20 30 40;do
	mpirun -np ${i} ./mpi_p	
done

