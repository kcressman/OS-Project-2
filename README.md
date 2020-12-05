## OS-Project-2 -- Memory Allocation Simulator

# Description:

This program is a simulation of memory allocation in several different scenarios. It consists of a simulated system of 4 processors that handle a total of 40 processes' memory allocation both with the C++ memory allocation library and our own implementation of the malloc and free processes. Please see our wiki for more information on the project!

# How to run the program:

To set a size limitation for the system (scenarios 2, 3, and 4), open mhelper.cpp and alter the two variables memory and memsize on lines 11 and 12 to the desired size and then re-compile project. The default system size is 10MB.

From the command line, run
g++ -o main main.cpp mhelper.cpp && ./main
