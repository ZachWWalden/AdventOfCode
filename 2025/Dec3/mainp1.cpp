#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

int getJoltage(std::string line);

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
	int sum = 0;

	while(std::getline(file, line))
	{
		// std::cout << line;
		sum += getJoltage(line);
	}

	std::cout << "Total Joltage = " << sum << std::endl;

	return 0;
}

int getJoltage(std::string line)
{
	//find max digit + it's position in all positions, except the last.
	//start past the max dig's pos and find the max digit.
	int max_tenths = 0;
	int max_tenths_pos = 0;
	for(int i = 0; i < line.length() - 1; i++)
	{
		if((line[i] - 0x30) > max_tenths)
		{
			max_tenths = line[i] - 0x30;
			max_tenths_pos = i;
		}
	}
	int max_ones = 0;
	for(int i = max_tenths_pos + 1; i < line.length(); i++)
	{
		if((line[i] - 0x30) >= max_ones)
		{
			max_ones = line[i] - 0x30;
		}
	}
	// std::cout << " Bank Joltage = " << 10 * max_tenths + max_ones << std::endl;
	return 10 * max_tenths + max_ones;
}
