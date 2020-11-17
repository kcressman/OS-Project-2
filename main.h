// main.h - header for main project file (main.cpp)

#include<iostream>
#include<random>
#include<queue>

using namespace std;

// structure definitions for processes and processors
struct Processor
{
	long long speed [4] = {3000000000,3000000000,3000000000,3000000000};
	int mem [4] = {10,10,10,10};
};

struct Process
{
	long long st;
	int mem;
};

// prototypes for functions
void menu();
void generateProcesses(queue<Process>&, int); 
