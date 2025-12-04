#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <cmath>

#define NUM_DIGS 12

uint64_t getJoltage(std::string line);

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
	uint64_t sum = 0;

	while(std::getline(file, line))
	{
		// std::cout << line;
		sum += getJoltage(line);
	}

	std::cout << "Total Joltage = " << sum << std::endl;

	return 0;
}

uint64_t getJoltage(std::string line)
{
	//find max digit + it's position in all positions, except the last.
	//start past the max dig's pos and find the max digit.
	int max_dig[NUM_DIGS];
	for(int i = 0; i < NUM_DIGS; i++)
		max_dig[i] = 0;

	int next_pos = 0;

	for(int i = NUM_DIGS - 1; i >= 0; i--)
	{
		for(int j = next_pos; j < line.length() - i; j++)
		{
			if((line[j] - 0x30) > max_dig[i])
			{
				max_dig[i] = line[j] - 0x30;
				next_pos = j + 1;
			}
		}
	}

	uint64_t sum = 0;

	for(int i = 0; i < NUM_DIGS; i++)
	{
		sum += max_dig[i] * pow(10, i);
	}

	// std::cout << " Bank Joltage = ";
	// for(int i = NUM_DIGS - 1; i >= 0; i--)
	// {
	// 	std::cout << max_dig[i];
	// }
	// std::cout << std::endl;

	return sum;
}
