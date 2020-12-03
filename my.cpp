#include<iostream>
#include<queue>
using namespace std;

queue<int> waiting; 		//smaller then memory but no space available
queue<int> rejected; 		//larger then memory
int inprogress[4];			//stores the process with index of tuple 
int processinprogress[8];	//stores tuples of start and end spots from malloc
int process[4] = {5,2,20,3};//trial process
int memory[10000];			//size of memory

void fillMemory();
void my_malloc(int m, int sysi);
void my_free(int x);
void checkIndex(int* x, int* y);

int main()
{
	fillMemory();
	
	int p = process[1];
	int sysi = 0;
	
	my_malloc(p, sysi);
	
	for (int i = 0; i < 10000; i++)//check if allocated correctly
	{
		cout<<memory[i]<<endl;
	}
	
	int x = sysi;	//which process are we bringing over when it is done running
	my_free(x);
	cout<<" "<<endl;
	for (int i = 0; i < 10000; i++)//check if deallocated correctly
	{
		cout<<memory[i]<<endl;
	}
	return 0;
}

//requires int of process memory
void my_malloc(int m, int sysi)
{
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
							break;
						}
						if (j == 9)//end of memory
						{
							end = j;
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