// main.h: header for main project file (main.cpp)

#include<iostream>
#include<random>
#include<queue>
#include<cstdlib>
#include<ctime>
#include<chrono>

using namespace std;

// structure definitions for processes and processors
struct Processor
{
	long long speed = 3000000000;
	bool inUse = false;
};

struct Process
{
	int id = -1; // this will be used to denote a placeholder process
	long long st; // service time
	int mem; 
	long long at = 0; // attained time
	int* ptr; // for malloc/free in scenario1
	int ptr2; //for my_malloc/my_free in scenario2
	int loc = -1; 
	// @loc: (for my_malloc/my_free) location in 10mb memory block where process enters
	// if -1, process wasn't allocated
};

// prototypes for functions
void menu();
void generateProcesses(queue<Process>&, int); 
void scenario1(int s);
void scenario2(int s);
void scenario3(int s);
void scenario4(int s);
void fillMemory();
int my_malloc(int m, int sysi);
void my_free(int x);
void checkIndex(int* x, int* y);