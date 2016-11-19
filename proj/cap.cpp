#include <iostream> 
#include <cmath> 
#include <cstdlib>

using namespace std; 

#define history 24

struct space
	{
	bool freq[4][history];
	}; 

struct device
{
	double accessProb[4][24];
	int currentCh;
};

double sigmoid(double input)
	{
	// produce val between 0 and 1
	return abs(input/(1 + abs(input)));
	}

int pickCh(device currentDev, int time)
{
	double highestProb = currentDev.accessProb[0][time];
	int chNum = 0;

	// all probabilities are same
	if(currentDev.accessProb[0][time] == currentDev.accessProb[1][time]
		&& currentDev.accessProb[0][time] == currentDev.accessProb[2][time]
		&& currentDev.accessProb[0][time] == currentDev.accessProb[3][time])
	{
		// pick random channel
		return rand() % 4;
	}
	// assume there is a unique max
	for(int i = 1; i < 4; i++)
	{
		if(currentDev.accessProb[i][time] > highestProb)
		{
			highestProb = currentDev.accessProb[i][time];
			chNum = i;
		}
	}

	return chNum;
}

int main()
{
	space one;
	device dev1;

	// initialize device access probability values
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < history; j++)
		{
			dev1.accessProb[i][j] = 0.5;
		}
	}
	
	// initialize channel access patterns
	for (int i = 0; i < 4; i++)
	{
		for(int j = 0; j < history; j++)
		{
			if(i == 0)
				one.freq[i][j] = round(3* sigmoid(sin(j+5) + 0.75) - 0.05);
			else if( i == 1)
				one.freq[i][j] = round(sigmoid(sin(2*j)/3 - 3) - 0.25);
			else if( i == 2)
				one.freq[i][j] = round(sigmoid(cos(j)/2) + 0.3);
			else
				one.freq[i][j] = round(sigmoid(2*cos((j+4))) - 0.1);
		}
	}

	// print channel access pattern - for comparison purposes only
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < history; j++)
		{
			cout << one.freq[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	// train that ho
	for(int i = 0; i < 500000; i++)
	{
		// initialize channel
		dev1.currentCh = pickCh(dev1, i % history);

		// check channel
		if(one.freq[dev1.currentCh][i % history] == 0)
			dev1.accessProb[dev1.currentCh][i % history] += 0.05;
		else
			dev1.accessProb[dev1.currentCh][i % history] -= 0.05;
	}

	// print probability matrix
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < history; j++)
		{
			cout << dev1.accessProb[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	return 0; 
}

