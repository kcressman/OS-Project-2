#include<iostream>
#include<queue>
using namespace std;

queue<int> waiting; 	//smaller then memory but no space available
queue<int> rejected; 	//larger then memory
int processinprogress[4];	//stores the process with index of tuple 
int inprogress[4];			//stores tuples of start and end spots from malloc
int process[4] = {5,2,20,3};	//trial process
int memory[10];		//trial memory

void fillMemory();
void my_malloc(int m);
void my_free(int m);

int main()
{
	fillMemory();
	//memory[0] = 1;
	memory[4] = 1;
	int p = process[1];
	
	if (p < 10)
	{//if the process is small enough to fit into memory
		my_malloc(p);
	}
	else
	{//if memory of process is bigger than the size add to queue
		rejected.push(p);
	}
	
	for (int i = 0; i < 10; i++)//check if allocated correctly
	{
		cout<<memory[i]<<endl;
	}
	//my_free(mm);
	return 0;
}

void my_malloc(int m)
{
	int start = 0;	//keep track of first available spot
	int end = 0;	//keep track of last available spot
	int tracker = 0;//track end of memory
	bool finish = false;
	while (!finish)
	{//check if there is space
		for (int i = tracker; i < 10; i++)
		{	
			if (memory[i] == 0)
			{//finds index of first available spot
				start = i;
				for (int j = i; j < 10; j++)
				{//find last available contingent spot
					if (memory[j] != 0)
					{
						end = j-1;
						break;
					}
					if (j == 9)//end of memory
					{
						end = j;
						tracker = 10;//break out of while loop
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
	cout<<"start "<<start<<" end "<<end<<endl;
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
	
}

void my_free(int p, int ip)
{//bring over process and tuple to free
	int newM = p - 5;
	cout<<"free "<<newM<<endl;

}

void fillMemory()
{
	for (int i = 0; i < 10; i++)
	{
		memory[i] = 0;
	}
}