// mhelper.cpp: functions for main project file (main.cpp)

#include "main.h"

queue<Process> waiting; 		//smaller then memory but no space available
queue<Process> rejected; 		//larger then memory
int inprogress[4];			//stores the process with index of tuple
int processinprogress[8];	//stores tuples of start and end spots from malloc

// ===========EDIT SYSTEM MEMORY SIZE HERE==============
int memory[1000];
int memsize = 1000;

// @menu: displays opening menu after prompting user for seed value
// @see fillMemory
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
				fillMemory(limit2);
				scenario2(seeed);
				break;
			case 3:	
				fillMemory(limit3);				
				scenario3(seeed);
				break;
			case 4:
				fillMemory(limit4);
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
				cout << "\nPlease choose a valid option.\n\n" << endl;
				break;
			}
	}
}

// generateProcesses: creates 40 processors with randomized memory requirements and service times
// @plist: mem address of queue to assign processes to
// @s: user selected seed for randomization
void generateProcesses(queue<Process>& plist, int s) {
	int totalmem = 0;
	unsigned long long int totst = 0;
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
		totst += serviceTime;
		temp.mem = nlist[i];
		temp.id = i + 1;
		plist.push(temp);
		totalmem += temp.mem;

		//cout << "Process " << temp.id << "\nST: " << temp.st << "\nMem: " << temp.mem << endl;
	}

	//cout << "total memory utilization of all processes: " << totalmem << "\n";
	//cout << "average service time: " << totst / 40 << "\n\n";
}

// procFree: used to determine if all four processors are free
// @param sys array of processors to evaluate
// @return boolean; true if a processor is in use, false if none are in use
bool procFree(Processor sys[]) {
	for(int i = 0; i < 4; i++) {
		if(sys[i].inUse) {
			return false;
		}
	}
	return true;
}

// scenario1: uses malloc/free to allocate memory for simulation
// @param s user selected seed for randomization
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
	cout<< "It took "<< time <<" ns to complete.\n\n";
}

// Scenario 234: uses my_malloc/my_free to allocate memory for simulation
// @param s user selected seed for randomization
// @see my_malloc
// @see my_free
// @see generateProcesses
// @see procFree
void scenario2(int s) {
	unsigned long long int cycle = 0;
	queue<Process> ready; // ready queue
	queue<Process> complete; // completed processes
	Process temp;
	Process ph; // placeholder process
	Process onProc[4] = {ph, ph, ph, ph};
	bool sysDone = false;
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
					temp.ptr2 = my_malloc(temp, i); // allocate memory in bytes
					onProc[i] = temp; // assign the next process to a processor
					sys[i].inUse = true; // mark processor in use

					cout << "Assigning PID " << onProc[i].id << " to processor #" << (i+1) << " (at cycle " << cycle << ")" << endl;
					//cout << "ST: " << onProc[i].st << endl;
					cout << "Memory Location: " << onProc[i].ptr2 << endl;
					cout << "Mem Requirements: " << onProc[i].mem << "\n\n";
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

				// a process left the system... time to check to see if a process is waiting
				if(!waiting.empty() && waiting.front().id != 0) {
					temp = waiting.front();
					ready.push(temp);
					waiting.pop();
					//cout << "added PID " << temp.id << " back to ready\n\n";
				}
			}
		}

		if(ready.empty() && procFree(sys) && !waiting.empty()) {
			temp = waiting.front();
			ready.push(temp);
			waiting.pop();
			//cout << "added PID " << temp.id << " back to ready\n\n";
		}

		minST = 10000000000001; // set min service time back to default

		// check if completed queue has reached 40 and no processors are in use
		if((ready.size() + waiting.size() < 1) && procFree(sys)) {
			sysDone = true;
		}
	}
	//time in nanoseconds displayed
	cout << "It took " << time << " ns to complete.\n\n";
}

void scenario3(int s) {
	unsigned long long int cycle = 0;
	queue<Process> ready; // ready queue
	queue<Process> complete; // completed processes
	Process temp;
	Process ph; // placeholder process
	Process onProc[4] = {ph, ph, ph, ph};
	bool sysDone = false;
	long long minST = 10000000000001;

	// generate processors with given seed
	generateProcesses(ready, s);

	// create processors
	Processor sys[4];

	cout << "\n= Scenario Three Start =" << endl;

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
					temp.ptr2 = my_malloc(temp, i); // allocate memory in bytes
					onProc[i] = temp; // assign the next process to a processor
					sys[i].inUse = true; // mark processor in use

					cout << "Assigning PID " << onProc[i].id << " to processor #" << (i+1) << " (at cycle " << cycle << ")" << endl;
					//cout << "ST: " << onProc[i].st << endl;
					cout << "Memory Location: " << onProc[i].ptr2 << endl;
					cout << "Mem Requirements: " << onProc[i].mem << "\n\n";
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

				// a process left the system... time to check to see if a process is waiting
				if(!waiting.empty() && waiting.front().id != 0) {
					temp = waiting.front();
					ready.push(temp);
					waiting.pop();
					//cout << "added PID " << temp.id << " back to ready\n\n";
				}
			}
		}

		if(ready.empty() && procFree(sys) && !waiting.empty()) {
			temp = waiting.front();
			ready.push(temp);
			waiting.pop();
			//cout << "added PID " << temp.id << " back to ready\n\n";
		}

		minST = 10000000000001; // set min service time back to default

		// check if completed queue has reached 40 and no processors are in use
		if((ready.size() + waiting.size() < 1) && procFree(sys)) {
			sysDone = true;
		}
	}
	//time in nanoseconds displayed
	cout << "It took " << time << " ns to complete.\n\n";
}

void scenario4(int s) {
	unsigned long long int cycle = 0;
	queue<Process> ready; // ready queue
	queue<Process> complete; // completed processes
	Process temp;
	Process ph; // placeholder process
	Process onProc[4] = {ph, ph, ph, ph};
	bool sysDone = false;
	long long minST = 10000000000001;

	// generate processors with given seed
	generateProcesses(ready, s);

	// create processors
	Processor sys[4];

	cout << "\n= Scenario Four Start =" << endl;

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
					temp.ptr2 = my_malloc(temp, i); // allocate memory in bytes
					onProc[i] = temp; // assign the next process to a processor
					sys[i].inUse = true; // mark processor in use

					cout << "Assigning PID " << onProc[i].id << " to processor #" << (i+1) << " (at cycle " << cycle << ")" << endl;
					//cout << "ST: " << onProc[i].st << endl;
					cout << "Memory Location: " << onProc[i].ptr2 << endl;
					cout << "Mem Requirements: " << onProc[i].mem << "\n\n";
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

				// a process left the system... time to check to see if a process is waiting
				if(!waiting.empty() && waiting.front().id != 0) {
					temp = waiting.front();
					ready.push(temp);
					waiting.pop();
					//cout << "added PID " << temp.id << " back to ready\n\n";
				}
			}
		}

		if(ready.empty() && procFree(sys) && !waiting.empty()) {
			temp = waiting.front();
			ready.push(temp);
			waiting.pop();
			//cout << "added PID " << temp.id << " back to ready\n\n";
		}

		minST = 10000000000001; // set min service time back to default

		// check if completed queue has reached 40 and no processors are in use
		if((ready.size() + waiting.size() < 1) && procFree(sys)) {
			sysDone = true;
		}
	}
	//time in nanoseconds displayed
	cout << "It took " << time << " ns to complete.\n\n";
}

int my_malloc(Process m, int sysi)
{
	int temp;
	if (m.mem < memsize)
	{//if the process is small enough to fit into memory
		int start = 0;	//keep track of first available spot
		int end = 0;	//keep track of last available spot
		int tracker = 0;//track end of memory
		bool finish = false;
		while (!finish)
		{//check if there is space
			for (int i = tracker; i < memsize; i++)
			{
				if (memory[i] == 0)
				{//finds index of first available spot
					start = i;
					for (int j = i; j < memsize; j++)
					{//find last available contingent spot
						if (memory[j] != 0)
						{
							end = j-1;
							break;
						}
						if (j == memsize - 1)//end of memory
						{
							end = j;
							tracker = memsize;//break out of while loop
							finish = true;
						}
					}
					break;
				}
			}
			if ((end - start)+1 < m.mem)
			{//checks if process has enough space in the current hole found
				tracker = end + 1;
			}
			else
			{
				finish = true;
			}
		}
		if ((end - start)+1 >= m.mem && (end - start)+1 >= 0)
		{//checks if spot is available and allocates to memory
			for(int i = start; i <= start + m.mem - 1; i++)
			{
				memory[i] = 1;
			}
		}
		else
		{ // if spot not available add to waiting queue
			waiting.push(m);
			cout << "Sent PID " << m.id << " to waiting queue.\n\n";
		}
		//store the process with the base and limit
		end = start + m.mem - 1;
		temp = end;
		int x = sysi;
		int y = 0;
		checkIndex(&x,&y);
		inprogress[sysi] = 1;
		processinprogress[x] = start;
		processinprogress[y] = end;
	}
	else
	{// mem requirement is too large! add to rejected queue
		rejected.push(m);
		cout << "Sent PID " << m.id << " to rejected queue.\n\n";
	}
	return temp;
}

// @my_free: when a process terminates, this function frees memory from a
// terminated process by setting the corresponding indices to 0
// @param p index on the memory array denoting location of process to free
void my_free(int p)
{//bring over process and tuple to free
	int x = p;
	int y = 0;
	checkIndex(&x,&y);
	inprogress[p] = 0;
	int start = processinprogress[x];
	int end = processinprogress[y];
	processinprogress[x] = 0;
	processinprogress[y] = 0;
	for(int i = start; i <= end; i++)
	{
		memory[i] = 0;
	}
}

// @fillMemory: initializes the array representing memory to all 0s
void fillMemory()
{
	for (int i = 0; i < memsize; i++)
	{
		//cout << i << endl;
		memory[i] = 0;
	}
}

// @checkIndex: returns postion of start and end of process related
// to processor holding it
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