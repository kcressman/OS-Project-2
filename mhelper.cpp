// mhelper.cpp - functions for main project file (main.cpp)

#include "main.h"

void menu() {
	// note: option 6 & queue pl will be removed later... just for testing

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
				cout << "scenario 1" << endl;
				break;
			case 2:
				cout << "scenario 2" << endl;
				break;
			case 3:					
				cout << "scenario 3" << endl;
				break;
			case 4:
				cout << "scenario 4" << endl;
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
		plist.push(temp);
		totalmem += temp.mem;
		
		cout << "Process " << (i + 1) << "\nST: " << temp.st << "\nMem: " << temp.mem << endl;
	}
	
	cout << "total memory utilization of all processes: " << totalmem << "\n\n";
}
