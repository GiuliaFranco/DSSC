# DSSC: Course of Parallel Programming 
This repository includes slides and material for the course of Parallel Programming at the https://dssc.units.it/

## Course Description 
The course aims to provide a foundation of parallel programming techniques commonly applied in scientific computing. Three different approaches are presented during clasess: a distributed memory paradigm using MPI, a shared memory paradigm using OpenMP and the CUDA language for programming NVIDIA based GPUs. The lecture series is part of the course of High-Performance Computing. It is mostly an hands-on based course, but including a mix of both direct lectures and practicals. Therefore, participation to classes is strongly recommended to successfully complete the course.

The course is divided in 5 lectures. Every lecture is expected to be from 10:00 to 12:30 and from 14:00 to 16:30, scheduled as follows:
- 2 March 2019, Giambiagi Lecture Hall (ICTP, Grignano)
- 29 March 2019, 5 April, 12 April and 3 May, Room D ex-SISSA Building (ICTP)
- 10 May 2019, Giambiagi Lecture Hall (ICTP, Grignano)  

## Lectures Description

### Day 1
During day1 basic notions of parallel programming have been introduced toghether with the essential sintaxt for writing a simple OpenMP program. Particular focus was given to the concept of thread, the OpenMP veriable scoping and finally the work shraing contruct for loops parallelization. 

Assignments: none  

Main references: [LLNL OpenMP Tutorial](https://computing.llnl.gov/tutorials/openMP/)

### Day 2
During day2 the both concepts of syncronizathion and race condition were mainly introduced, including how those are handled in OpenMP. 

Assignments: [OpenMP Assignements](https://github.com/igirotto/DSSC/blob/master/Lab/Day2/openmp_assignments.pdf)  

Main references: [LLNL OpenMP Tutorial](https://computing.llnl.gov/tutorials/openMP/)

### Day 3
During day3 basic notions of parallel programming for distributed memory system have been introduced toghether with the essential sintaxt for writing a simple MPI program. Particular focus was given to the execution flow of an MPI program as well as the concept of message and the MPI communicators. The MPI main models of communication were introduced such as blocking point-to-point and collective communications in the form of both the reduction and the broadcast operations.  

Assignments:  
- Compute the approximation of PI using the midpoint method (with a REALLY large number of rectangles). Reduce the final result in the last process (size – 1) and print the final output from 0! Compare timing with the OpenMP version, scaling the MPI version up to 2 nodes of Ulysses (excluding operation needed for I/O). Reccomendation: use 101 as MPI_TAG.  

Main references: [MPI 1.1 Documentation](https://www.mpi-forum.org/docs/mpi-1.1/mpi-11-html/mpi-report.html), [MPI 1.1 APIs description](https://www.mpi-forum.org/docs/mpi-1.1/mpi-11-html/node182.html).

### Day 4
During day4 an overview of best practises of parallel programming for distributed memory system was given with particular focus on how to approach a parallel problem that require data exchange between neighbour processes, local indexing Vs global indexing, how to handle ressts effiently, sequentialized I/O from the root process, taks farming, domain decomposition, glocal data Vs replicated data and non-blocking MPI communication.      

Assignments:  
- Implement a code to initialize a distributed identity matrix of size (N,N). Print the matrix ordered on standard output if N is smaller than 10, otherwise on a binary file. (Plus) Implement the I/O overlapping the receiving data on process 0 with no-blocking communication, therefore overlapping I/O operations on disk with data echange between the processes. 

- Implement a communication pattern of all_reduce using non-blocking point to point communication, first exchanging one single element (i.e., the rank Id of a given process) among processes. Try to optimize the code for sending in the ring a large set of data and
overlapping the computation (Σ) and the communication (send-recv). In case of a dataset larger than one element the local sum is
considered a vector sum (element by element).

Main references: [MPI 1.1 Documentation](https://www.mpi-forum.org/docs/mpi-1.1/mpi-11-html/mpi-report.html), [MPI 1.1 APIs description](https://www.mpi-forum.org/docs/mpi-1.1/mpi-11-html/node182.html).

### Day 5
Introduction to GPU computing and the CUDA language.     
