// mhelper.cpp: functions for main project file (main.cpp)

#include "main.h"

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

void scenario2(int s) {
	cout << "scenario 2" << endl;
}

void scenario3(int s) {
	cout << "scenario 2" << endl;
}

void scenario4(int s) {
	cout << "scenario 2" << endl;
}