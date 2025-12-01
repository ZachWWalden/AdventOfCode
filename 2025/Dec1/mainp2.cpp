#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <cmath>

void parseLine(bool& direction, int& distance, std::string line);
int rotateDial(int& dialLoc, bool direction, int rotDistance);

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		std::cout << "Crap" << std::endl;
		return -1;
	}

	std::string filename = argv[1];

	std::fstream file;
	file.open(filename, std::ios::in);

	std::string line;
	int sum = 0, location = 50;

	while(std::getline(file, line))
	{
		//parse line
		bool direction;
		int rotDistance;
		// std::cout << line << std::endl;
		parseLine(direction, rotDistance, line);
		if(rotDistance == -1)
		{
			std::cout << "Parsing error" << std::endl;
			break;
		}
		//perform rotation
		int numZero = rotateDial(location, direction, rotDistance);
		// std::cout << "Num zero = " << numZero << ", Loc = " << location << std::endl;
		sum += numZero;
		//check state
		// std::cout << location << std::endl;
		//if(location == 0)
			//sum++;
	}

	std::cout << "Password = " << sum << std::endl;

	return 0;
}

void parseLine(bool& direction, int& distance, std::string line)
{
	//get direction
	if(line[0] == 'L')
		direction = false;
	else if (line[0] == 'R')
		direction = true;
	else
	{
		direction = false;
		distance = -1;
		std::cout << "Error invalid direction specifier." << std::endl;
	}
	//loop over distance. convert from base 10 to base 2.
	int order = line.length() - 2;
	int sum = 0;
	for(int i = 1; i < line.length(); i++)
	{
		if(line[i] >= 0x30 && line[i] <= 0x39)
		{
			int dig = line[i] - 0x30;
			// std::cout << dig << std::endl;
			sum += dig * pow(10, order);
			// std::cout << sum << std::endl;
			order--;
		}
		else
		{
			std::cout << "Invalid non digit character following direction specifier." << std::endl;
			return;
		}
	}
	// std::cout << sum << std::endl;
	distance = sum;
}

int rotateDial(int& dialLoc, bool direction, int rotDistance)
{
	int numZero = 0;
	//ensure that rotDistance <= 100
	while(rotDistance > 100)
	{
		rotDistance -= 100;
		numZero++;
		// std::cout << numZero << std::endl;
	}

	int newLoc;
	//4 cases
	if(direction)
	{
		if(dialLoc + rotDistance > 99)
		{
			//addition, overflow
			newLoc = dialLoc + rotDistance - 99 - 1;
			if(dialLoc != 0)
				numZero++;
		}
		//addition, no overflow
		else
		{
			newLoc = dialLoc + rotDistance;
			if(newLoc == 0)
				numZero++;
		}
	}
	else
	{
		if(dialLoc - rotDistance < 0)
		{
			//subtraction, underflow
			newLoc = 1 + 99 + dialLoc - rotDistance;
			if(dialLoc != 0)
				numZero++;
		}
		//subtraction, no underflow
		else
		{
			newLoc = dialLoc - rotDistance;
			if(newLoc == 0)
				numZero++;
		}
	}
	dialLoc = newLoc;
	return numZero;
}
