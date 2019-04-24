#!/bin/bash

let MY_ID=${OMPI_COMM_WORLD_RANK}+1

#runs di example where different processes create different folders and different files 
mkdir dir$MY_ID
cd dir$MY_ID

# runs di example where different processes log different output
# depending by their ID. The example is meant to run the compiled prog.c 
# the ID is readed by the SHELL and passed to the programme in the form of 
# a command line argument  
#
../a.out $MY_ID >> file_$MY_ID
