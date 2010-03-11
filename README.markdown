#Overview

A simple example illustrating C++ Message passing interface. At a low level this can be super powerful to compute large data sets. I use some of these concepts for backend of some web based production apps.
The example illustrates asynchronous passing, since I'm computing something that is dependent on previous results, but this really shines asynchronously. (1 extra param to the MPI_Recv function) 

#Usage

Compile using "mpiCC -o sampleMatrix.out sampleMatrix.cpp"