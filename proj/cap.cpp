#include <iostream> 
#include <cmath> 
#include <cstdlib>
#include <stdlib.h>  
#include <time.h>

using namespace std; 

#define history 24
#define pi  3.141592653589793238463

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
		return rand()%4; 
	}

	// Else there is a unique max probability
	for(int i = 1; i <= 4; i++)
	{
		if(currentDev.accessProb[i][time] > highestProb)
		{
			highestProb = currentDev.accessProb[i][time];
			chNum = i;
		}
	}

	return chNum;
}

bool checkCollision(device deviceArray[], int numDevs)
{
	// return true if there is collision, false if not
	// get highest prob channels of two devices
	int size = numDevs;
	int hp[size];

	for(int i = 0; i < size; i++)
	{
		hp[i] = deviceArray[i].currentCh;
	}

	if(hp[0] == hp[1])
	{
		return true;
	}

	return false;
}

void initPrimaryUsers(space *channels, int pattern)
	{ 
	// initialize channel access patterns 
	switch(pattern)
		{
		// pattern 0: daily cycles 
		case 0: 
		for (int i = 0; i < 4; i++)
			{
			for(int j = 0; j < history; j++)
				{
				if(i == 0)
					channels->freq[i][j] = round(3*sin(pi*j/12)/4);
				else if( i == 1)
					channels->freq[i][j] = round(3*cos(pi*j/6)/4);
				else if( i == 2)
					channels->freq[i][j] = round(3*sin( (pi*j/12) + 8)/4 );
				else
					channels->freq[i][j] = round(3*sin( (pi*j/6) + 12)/4 );
				}
			}
		break; 

		// pattern 1: random user access 
		case 1: 
		for (int i = 0; i < 4; i++)
			{
			for(int j = 0; j < history; j++)
				{
				if(i == 0)
					channels->freq[i][j] = rand()%2;
				else if( i == 1)
					channels->freq[i][j] = rand()%2;
				else if( i == 2)
					channels->freq[i][j] = rand()%2;
				else
					channels->freq[i][j] = rand()%2;
				}
			}
		break; 
		}	
	}

int main()
{
	space one;
	device dev[2];
	int numCollisions = 0; 

	// Seed random number generator 
	srand(time(NULL)); 

	// initialize device access probability values
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < history; j++)
		{
			dev[0].accessProb[i][j] = 0.5;
			dev[1].accessProb[i][j] = 0.5;
		}
	}

	// Initialize access patterns 
	initPrimaryUsers(&one, 0); 

	// print channel access pattern - for comparison purposes only
	cout << "Primary User Access - 1 Denotes the channel is being accessed by primary user" << endl;
	for(int i = 0; i < 4; i++)
	{
	cout << "Channel " << i+1 << ": "; 
		for(int j = 0; j < history; j++)
		{
			cout << one.freq[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	// Run initial test 
	for (int j = 1; j < 24; ++j)
		{
		// Pick channels
		dev[0].currentCh = pickCh(dev[0], j % history);
		dev[1].currentCh = pickCh(dev[1], j % history);

		// Check for collision 
		if(checkCollision(dev, (sizeof(dev) / sizeof(dev[0]))))
			numCollisions++; 
		}
	cout << "Number of Collisions before training: " << numCollisions << endl; 

	// train devices
	cout << "Training: "<< endl;
	for(int i = 0; i < 100; i++)
	{
		// initialize channel
		dev[0].currentCh = pickCh(dev[0], i % history);
		dev[1].currentCh = pickCh(dev[1], i % history);

		if(checkCollision(dev, (sizeof(dev) / sizeof(dev[0]))))
		{
			//cout << "collision detected @ " << i << endl;
			dev[0].accessProb[dev[0].currentCh][i % history] -= 0.05;
			dev[1].accessProb[dev[1].currentCh][i % history] -= 0.05;
		}
		else
		{
			// check channel
				// dev 0
			if(one.freq[dev[0].currentCh][i % history] == 0)
				dev[0].accessProb[dev[0].currentCh][i % history] += 0.05;
			else
				dev[0].accessProb[dev[0].currentCh][i % history] -= 0.05;
				// dev 1
			if(one.freq[dev[1].currentCh][i % history] == 0)
				dev[1].accessProb[dev[1].currentCh][i % history] += 0.05;
			else
				dev[1].accessProb[dev[1].currentCh][i % history] -= 0.05;
		}
	}

	// print probability matrix
	for(int n = 0; n < 2; n++)
	{
		cout << "Device " << n+1 << ": " << endl; 
		for(int i = 0; i < 4; i++)
		{
			cout << "Channel " << i+1 << ": "; 
			for(int j = 0; j < history; j++)
			{
				cout << dev[n].accessProb[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl << endl;
	}

	// Run trained test
	numCollisions = 0;  
	for (int h = 1; h < 24; ++h)
		{
		// Pick channels
		dev[0].currentCh = pickCh(dev[0], h % history);
		dev[1].currentCh = pickCh(dev[1], h % history);

		// Check for collision 
		if(checkCollision(dev, (sizeof(dev) / sizeof(dev[0]))))
			numCollisions++; 
		}
	cout << "Number of Collisions after training: " << numCollisions << endl; 
	return 0; 
}


