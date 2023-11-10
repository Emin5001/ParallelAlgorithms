# Parallel Algorithms

## How to Run
In order to run a certain file, run the following command:
`make EXECUTABLE_NAME`, where `EXECUTABLE_NAME` is the `.cpp` file WITHOUT the `.cpp` extension.

Afterwards, just run `./EXECUTABLE_NAME`.

## Contents
### Parallel Prefix Sum
This repository mainly stores an implementation of the Prefix Sum aglorithm in 
the `prefix_sum.cpp` file. This implementation is computed using the POSIX pthreads
library, in which the computations are done in parallel. 

### Parallel Iterative Pi Computations
This repository includes implementations of parallel `C++` code that computes the value
of $\pi$ in parallel. 
The `prll_pi_nosync.cpp` file shows the negative effects of no synchronization methods,
and the other `*_pi.cpp` files showcase correct implementations that both use and don't 
use synchronization methods. 