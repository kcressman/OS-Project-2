// mhelper.cpp: functions for main project file (main.cpp)

#include "main.h"

queue<int> waiting; 		//smaller then memory but no space available
queue<int> rejected; 		//larger then memory
int inprogress[4];			//stores the process with index of tuple 
int processinprogress[8];	//stores tuples of start and end spots from malloc
int memory[10000];			//size of memory

void fillMemory();
int my_malloc(int m, int sysi);
void my_free(int x);
void checkIndex(int* x, int* y);

void menu() {
	int option;
	int seeed = 0;
	queue<Process> pl;
	
	cout << "Choose a seed for randomness: ";
	cin >> seeed;
	
	cout << "\n========\nWelcome!\n========\n";
	
	while(option != 5) {
		cout << "Please select an option:" << endl;
		cout << "1. Scenario 1" << endl;
		cout << "2. Scenario 2" << endl;
		cout << "3. Scenario 3" << endl;
		cout << "4. Scenario 4" << endl;
		cout << "5. Quit" << endl;
		cout << "6. Test random process generator" << endl;
		
		cin >> option;
		
		switch(option) {
			case 1:
				scenario1(seeed);
				break;
			case 2:
				fillMemory();
				scenario2(seeed);
				break;
			case 3:					
				scenario3(seeed);
				break;
			case 4:
				scenario4(seeed);
				break;			
			case 5:
				cout << "Goodbye!" << endl;
				break;
			case 6:
				generateProcesses(pl, seeed);
				break;
			default:
				// for invalid characters, options, etc: clear the input stream
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Please choose a valid option.\n\n" << endl;
				break;
			}
	}
}

// generateProcesses: creates 40 processors with randomized memory requirements and service times
// @plist: mem address of queue to assign processes to
// @s: user selected seed for randomization
void generateProcesses(queue<Process>& plist, int s) {
	int totalmem = 0;
	int memory;
	long int serviceTime;
	mt19937_64 randNum(s);
	Process temp;
	int nlist[40];
	bool done = false;
	int counter = 0;
	
	// initialize the list
	for(int i = 0; i < 40; i++) { 
		nlist[i] = 0; 
	}
	
	// making numbers for 10kb distributed memory
	// loops through, adding random values 1-100 to values until totalmem > 10000
	while(!done) {
		uniform_int_distribution<int> randMem(1, 100);
		memory = randMem(randNum);
		nlist[counter] += memory;
		totalmem += memory;
		counter++;
		
		// reset counter if necessary
		if(counter == 40) {
			counter = 0;
		} 
		
		// test for break from while (if total memory > 10k)
		// if it went over, deduct from whatever process the counter is on
		if(totalmem >= 10000) {
			nlist[counter] += 10000 - totalmem;
			done = true;
		}
	}
	
	totalmem = 0;
	
	for (int i = 0; i < 40; i++) {
		uniform_int_distribution<long int> randST(10000000, 10000000000000);
		serviceTime = randST(randNum);
		
		// set temp process' attributes and push to queue
		temp.st = serviceTime;
		temp.mem = nlist[i];
		temp.id = i + 1;
		plist.push(temp);
		totalmem += temp.mem;
		
		//cout << "Process " << temp.id << "\nST: " << temp.st << "\nMem: " << temp.mem << endl;
	}
	
	//cout << "total memory utilization of all processes: " << totalmem << "\n\n";
}

// procFree: used to determine if all four processors are free
// @sys: array of processors to evaluate
bool procFree(Processor sys[]) {
	for(int i = 0; i < 4; i++) {
		if(sys[i].inUse) {
			return false;
		}
	}
	return true;
}

// scenario1: uses malloc/free to allocate memory for simulation
// @s: user selected seed for randomization
// @see generateProcesses
// @see procFree
void scenario1(int s) {
	unsigned long long int cycle = 0;
	queue<Process> ready; // ready queue
	queue<Process> complete; // completed processes 
	Process temp;
	Process ph; // placeholder process
	Process onProc[4] = {ph, ph, ph, ph};
	bool sysDone = false;
	int memory = 10000;
	long long minST = 10000000000001;
	
	// generate processors with given seed
	generateProcesses(ready, s);
	
	// create processors
	Processor sys[4];
	
	cout << "\n= Scenario One Start =" << endl;
	
	//start timer 
	clock_t timer = clock();
	int time;
	// main processing loop
	while(!sysDone) {
		time = (clock()-timer)/double(CLOCKS_PER_SEC)*1000000000;
		// check if ready queue has anything to add
		if(!ready.empty()) {
			// check all four processes to see if one is available
			for(int i = 0; i < 4; i++) {
				if(!sys[i].inUse && ready.size() != 0) { 
					//first element process in ready queue
					temp = ready.front();
					ready.pop(); // pop the next process off
					temp.ptr = (int*) malloc(temp.mem * 1000); // allocate memory in bytes
					onProc[i] = temp; // assign the next process to a processor
					sys[i].inUse = true; // mark processor in use

					cout << "Assigning PID " << onProc[i].id << " to processor #" << (i+1) << " (at cycle " << cycle << ")" << endl;
					cout << "ST: " << onProc[i].st << endl;
					cout << "Memory Location: " << onProc[i].ptr << "\n\n";
				}
			}
		}
		
		// get minimum # of cycles til next process leaves
		for(int i = 0; i < 4; i++) {
			if(onProc[i].id > 0 && onProc[i].st - onProc[i].at < minST) {
				minST = onProc[i].st - onProc[i].at;
			}
		}
		
		// add that min # of cycles to everyone's attained time
		cycle += minST;
		for(int i = 0; i < 4; i++) {
			onProc[i].at += minST;
			if(onProc[i].id > 0 && onProc[i].st == onProc[i].at) {
				sys[i].inUse = false; // free processor
				free(onProc[i].ptr); // free memory allocated to process
				complete.push(onProc[i]); // process is complete!
				cout << "PID " << onProc[i].id << " is complete.\n\n";
				onProc[i] = ph; // set placeholder
			}
		}
		minST = 10000000000001; // set min service time back to default
		
		// check if completed queue has reached 40 and no processors are in use
		if(complete.size() == 40 && procFree(sys)) {
			sysDone = true;
		}
	}
	//time in nanoseconds displayed
	cout<< "It took "<< time <<" ns to complete.";
}

void scenario2(int s) {
	unsigned long long int cycle = 0;
	queue<Process> ready; // ready queue
	queue<Process> complete; // completed processes 
	Process temp;
	Process ph; // placeholder process
	Process onProc[4] = {ph, ph, ph, ph};
	bool sysDone = false;
	int memory = 10000;
	long long minST = 10000000000001;
	
	// generate processors with given seed
	generateProcesses(ready, s);
	
	// create processors
	Processor sys[4];
	
	cout << "\n= Scenario Two Start =" << endl;
	
	//start timer 
	clock_t timer = clock();
	int time;
	// main processing loop
	while(!sysDone) {
		time = (clock()-timer)/double(CLOCKS_PER_SEC)*1000000000;
		// check if ready queue has anything to add
		if(!ready.empty()) {
			// check all four processes to see if one is available
			for(int i = 0; i < 4; i++) {
				if(!sys[i].inUse && ready.size() != 0) { 
					//first element process in ready queue
					temp = ready.front();
					ready.pop(); // pop the next process off
					//ptr to first element in ready queue
					//mem = memory size of process
					//allocating space on heap to store an int
					temp.ptr2 = my_malloc(temp.mem, i); // allocate memory in bytes
					onProc[i] = temp; // assign the next process to a processor
					sys[i].inUse = true; // mark processor in use

					cout << "Assigning PID " << onProc[i].id << " to processor #" << (i+1) << " (at cycle " << cycle << ")" << endl;
					cout << "ST: " << onProc[i].st << endl;
					cout << "Memory Location: " << onProc[i].ptr2 << "\n\n";
				}
			}
		}
		
		// get minimum # of cycles til next process leaves
		for(int i = 0; i < 4; i++) {
			if(onProc[i].id > 0 && onProc[i].st - onProc[i].at < minST) {
				minST = onProc[i].st - onProc[i].at;
			}
		}
		
		// add that min # of cycles to everyone's attained time
		cycle += minST;
		for(int i = 0; i < 4; i++) {
			//process id set to min# of cycles for preemption
			onProc[i].at += minST;
			//if all service time is gone
			if(onProc[i].id > 0 && onProc[i].st == onProc[i].at) {
				sys[i].inUse = false; // free processor
				my_free(onProc[i].ptr2); // free memory allocated to process
				complete.push(onProc[i]); // process is complete!
				cout << "PID " << onProc[i].id << " is complete.\n\n";
				onProc[i] = ph; // set placeholder
			}
		}
		minST = 10000000000001; // set min service time back to default
		
		// check if completed queue has reached 40 and no processors are in use
		if(complete.size() == 40 && procFree(sys)) {
			sysDone = true;
		}
	}
	//time in nanoseconds displayed
	cout<< "It took "<< time <<" ns to complete.";
}

void scenario3(int s) {
	cout << "scenario 2" << endl;
}

void scenario4(int s) {
	cout << "scenario 2" << endl;
}

int my_malloc(int m, int sysi)
{
	int temp;
	if (m < 10000)
	{//if the process is small enough to fit into memory
		int start = 0;	//keep track of first available spot
		int end = 0;	//keep track of last available spot
		int tracker = 0;//track end of memory
		bool finish = false;
		while (!finish)
		{//check if there is space
			for (int i = tracker; i < 10000; i++)
			{	
				if (memory[i] == 0)
				{//finds index of first available spot
					start = i;
					for (int j = i; j < 10000; j++)
					{//find last available contingent spot
						if (memory[j] != 0)
						{
							end = j-1;
							temp = j-1;
							break;
						}
						if (j == 9)//end of memory
						{
							end = j;
							temp = j;
							tracker = 10000;//break out of while loop
							finish = true;
						}
					}
					break;
				}
			}
			if ((end - start)+1 < m)
			{//checks if process has enough space in the current hole found
				tracker = end + 1;
			}
			else
			{
				finish = true;
			}
		}
		if ((end - start)+1 >= m && (end - start)+1 >= 0)
		{//checks if spot is available and allocates to memory
			for(int i = start; i <= start + m - 1; i++)
			{
				memory[i] = 1;
			}
		}
		else
		{//if spot not available add to waiting queue
			waiting.push(m);
		}
		//store the process with the base and limit
		end = start + m - 1;
		
		int x = sysi;
		int y = 0;
		//returns postion of start and end of process related to processor holding it
		checkIndex(&x,&y);
		inprogress[sysi] = 1;
		processinprogress[x] = start;
		processinprogress[y] = end;
	}
	else
	{//if memory of process is bigger than the size add to queue
		rejected.push(m);
	}
	return temp;
}

//requires process and processor
void my_free(int p)
{//bring over process and tuple to free
	int x = p;
	int y = 0;
	checkIndex(&x,&y);
	inprogress[p] = 0;	//turns to 0 to show available
	int start = processinprogress[x];
	int end = processinprogress[y];
	processinprogress[x] = 0;
	processinprogress[y] = 0;
	for(int i = start; i <= end; i++)
	{
		memory[i] = 0;
	} 
}

void fillMemory()
{
	for (int i = 0; i < 10000; i++)
	{
		memory[i] = 0;
	}
}

void checkIndex(int* x, int* y)
{
	int i = *x;
	if (i == 0)
	{
		*x = 0;
		*y = 1; 
	}
	else if (i == 1)
	{
		*x = 2;
		*y = 3; 
	}
	else if (i == 2)
	{
		*x = 4;
		*y = 5; 
	}
	else if (i == 3)
	{
		*x = 6;
		*y = 7; 
	}
}