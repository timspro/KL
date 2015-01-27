This is the parallel implementation of the KLFatC algorithm.
Please see the documentation for the KLFatC algorithm, which is better documented.
Note that this does not make use of PolyStore.

The main differences are:
1. We now find nus in parallel.
2. We now computed FatC_nu in parallel.
3. We now compute FatC_gamma - g(t)FatC_nu in parallel.