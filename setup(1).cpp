// Group 2 3: Sam F and Keila C
// Project 2
// Date: 11/20/2020

#include<iostream>
#include<random>
#include<vector>
using namespace std;

struct Processor
{
	long long speed [4] = {3000000000,3000000000,3000000000,3000000000};
	int mem [4] = {10,10,10,10};
};

struct Processes
{
	long long st [40];
	int memkb [40]; 	//make 40 out of 10,000
};

int main()
{
	struct Processes pcess;
	int x = 0;
	int total = 0;
	//generates random service and memory for 40 processes
	mt19937_64 randNum(x);
	uniform_int_distribution<long long> randST(10000000, 10000000000000);
	uniform_int_distribution<int> randTime(1,250);
	
	for (int i = x; i < x+40; i++)
	{
		long long serviceTime = randST(randNum);	
		
		int memory = randTime(randNum);
		
		total += memory;
		//inputs random service times and memory
		pcess.st[i] = serviceTime;
		pcess.memkb[i] = memory;
	}
	
	//find difference of 10mb from total and place in last index of array
	int remainder = (10000 - total) + pcess.memkb[39];
	pcess.memkb[39] = remainder;
	
	/*int total2 = 0;
	for(int i =0;i<40;i++)
	{
		cout<<i+1<<" : "<<pcess.memkb[i]<<endl;
		total2 += pcess.memkb[i];
	}
	cout<<total2;*/
	return 0;
}