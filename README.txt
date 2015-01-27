Tim Sprowl
tms7qy@virginia.edu

KL Fat C Algorithm:

Compilation is straightforward and can be done with either MSVC or g++. Simply compile all C++ files together.

The produced binary can be executed as normal, but it will need the graph file of the form graph-X,X.film to be in the same directory as the executable, where X is the value of RANK in Globals.h. There is also a graph-9,9.film that is not included due to size.

To use different test cases, change RANK in the Globals.h file. This will automatically change what graph file the program looks for and the maximum weight used in the program. By default, RANK is set to 7, which means it will use graph-7,7.film and (5,5,5,5,5,5).

The output will be in the file in the form of output-X,X.kl. It is a text file that can be browsed with any text editor. By default, the output file will be output-7,7.kl.

I have also included reference-5,5.txt and reference-7,7.txt. These are known correct outputs for RANK=5 and RANK=7 respectively.

Look at KLFatC.pdf for information about the actual algorithm and what the input and output files actually mean.

I have also included the folder KLFatCOld, which has the C++ files before I added the PolyStore. Compiling and executing that code is pretty much the same. However, the code is sparesly commented (although very similar to the main commented code).