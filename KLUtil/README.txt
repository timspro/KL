Tim Sprowl
tms7qy@virginia.edu

KL Graph Generation:

This program generates the graph files that describe the group relations and is to be used with the KLFatC program. To compile this program, rename the desired .h file to globals.h. For example, for n=5, rename globals-5.h to globals.h. Then, simply compile the program by issuing a command such as "g++ *.cpp -O3 -o KL.out". The output will be a file called graph-X,X.film, where X is the desired n. To use this with the KL Fat C algorithm, simply make sure that it is in the same directory as the executable and then compile and run the executable.