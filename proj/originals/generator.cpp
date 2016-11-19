#include <iostream> 
#include <cmath> 

using namespace std; 

#define history 25

struct space
	{
	bool freq1[history]; 
	bool freq2[history];
	bool freq3[history]; 
	bool freq4[history]; 
	}; 

struct device
{
	// access probablility
		// first index: channel #
		// second index: sum of times accessed
	int access[4][2];
};

double sigmoid(double input)
	{
	// Mat's sigmoid
	return abs(input/(1 + abs(input)));
	/*
	Real sigmoid
	return abs(1/(1 + exp(0-input))); 
	*/
	}

int highestProb(device currentDevice)
{
	// initialize highest probability channel and iterator
	int hp = 0, i;

	// loop through available channels
	for(i = 0; i < 4; i++)
	{
		// compare current channel probability against previous highest
		if(currentDevice.access[i][1] > hp)
		{
			// change highest probability channel #
			hp = currentDevice.access[i][0];
		}
	}

	return hp;
}

int tryNext(device currentDevice)
{
	// get highest probability channel
	return highestProb(currentDevice);
}

int main()
	{
	space one;/*, two, three, four; */
	int currentChannel = 0;
	device dev1;

	// initialize device values
	for(int i = 0; i < 4; i++)
	{
		dev1.access[i][0] = i;
		dev1.access[i][1] = 0;
	}

	for (int i = 0; i < history; i++)
		{
		// First Space 
		one.freq1[i] = round(3* sigmoid(sin(i+5) + 0.75) - 0.05); 
		one.freq2[i] = round(sigmoid(sin(2*i)/3 - 3) - 0.25); 
		one.freq3[i] = round(sigmoid(cos(i)/2) + 0.3); 
		one.freq4[i] = round(sigmoid(2*cos((i+4))) - 0.1); 
		}

	// print channel access pattern - for comparison purposes only
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < history; j++)
		{
			switch(i)
			{
				case 0:
					cout << j % 10 << " ";
					if(j == history - 1)
						cout << endl;
					break;
				case 1:
					cout << one.freq1[j] << " ";
					break;
				case 2:
					cout << one.freq2[j] << " ";
					break;
				case 3:
					cout << one.freq3[j] << " ";
					break;
				case 4:
					cout << one.freq4[j] << " ";
					break;
			}
		}
		cout << endl;
	}
	cout << endl;

	for(int i = 0; i < 500000; i++)
	{
		// check channel
		switch(currentChannel)
		{
			case 0:
					if(one.freq1[i % history] == 0)
						dev1.access[0][1]++;
					else
						currentChannel++;
					break;
			case 1:
					if(one.freq2[i % history] == 0)
						dev1.access[1][1]++;
					else
						currentChannel++;
					break;
			case 2:
					if(one.freq3[i % history] == 0)
						dev1.access[2][1]++;
					else
						currentChannel++;
					break;
			case 3:
					if(one.freq4[i % history] == 0)
						dev1.access[3][1]++;
					else
						currentChannel = 0;
					break;
			default:
					cout << "The fuck did you do? Channel #: " << currentChannel << endl;
					break;
		}

		// adjust based on learned history
		if((i % 10) == 0 && i != 0)
			currentChannel = tryNext(dev1);

	}

	int sumOfAccesses = 0;
	// print channel access probabilities
	for(int i = 0; i < 4; i++)
	{
		cout << "Channel: " << i + 1 << " Access Probability: " << dev1.access[i][1] << endl;
		sumOfAccesses += dev1.access[i][1];
	}
	
	cout << "Number of Accesses: " << sumOfAccesses << endl;

	return 0; 
	}


