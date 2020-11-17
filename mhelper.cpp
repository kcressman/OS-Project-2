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
	int finaltotal = 0;
	int memory;
	long int serviceTime;
	mt19937_64 randNum(s);
	Process temp;
	
	for (int i = 0; i < 40; i++) {
		uniform_int_distribution<long int> randST(10000000, 10000000000000);
		serviceTime = randST(randNum);
		
		if (i == 39) { // if it's the last process, assign leftoever memory
			memory = 10000 - totalmem;
		} else { // or generate a random memory value to assign
			uniform_int_distribution<int> randTime(50,400);
			memory = randTime(randNum);
			totalmem += memory;
		}

		finaltotal += memory;		
		
		// set temp process' attributes and push to queue
		temp.st = serviceTime;
		temp.mem = memory;
		plist.push(temp);
		
		cout << "Process " << (i + 1) << "\nST: " << temp.st << "\nMem: " << temp.mem << endl;
	}
	
	cout << "total memory utilization of all processes: " << finaltotal << "\n\n";
}
