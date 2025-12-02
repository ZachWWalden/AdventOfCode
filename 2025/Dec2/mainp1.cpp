#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Range
{
	uint64_t start, end;
};

std::vector<std::string>* splitString(std::string string, char delimeter);
Range parseRange(std::string range_str);
uint64_t validateRange(Range range);

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

	std::vector<int64_t> left, right;
	std::vector<std::string>* unparsed_ranges;

	while(std::getline(file, line))
	{
		unparsed_ranges = splitString(line, ',');
	}

	for(std::string rng : (*unparsed_ranges))
	{
		sum += validateRange(parseRange(rng));
	}

	std::cout << "Invalid Sum = " << sum << std::endl;

	return 0;
}

std::vector<std::string>* splitString(std::string string, char delimeter)
{
	std::vector<std::string>* split = new std::vector<std::string>;

	std::size_t pos = 0;
	std::string splitVal;

	while (pos != std::string::npos)
	{
		//std::cout << string << std::endl;
		pos = string.find(delimeter);
		//2 sub str, the
		splitVal = string.substr(0, pos);
		if(splitVal.length() > 0)
			split->push_back(splitVal); //push_back 1st token separated by "delimeter"
		string = string.substr(pos + 1);
	}

	/*
	std::vector<std::string>::iterator j;
	for(j = split->begin(); j < split->end();j++)
	{
		std::cout << (*j) << std::endl;
	}
	*/
	return split;
}

Range parseRange(std::string range_str)
{
	Range rng;
	//split string on '-'
	std::vector<std::string>* values = splitString(range_str, '-');
	//convert start and end values to uint64_t.
	rng.start = std::stoull((*values)[0], nullptr, 10);
	rng.end = std::stoull((*values)[1], nullptr, 10);
	return rng;
}

uint64_t validateRange(Range range)
{
	uint64_t sum = 0;
	//loop through the range
	for(uint64_t i = range.start; i <= range.end; i++)
	{
		//convert to string
		std::string num = std::to_string(i);
		//if string length is even check for equivalency
		if(num.length() % 2 == 0)
		{
			//check for a repeating pattern.
			int valid = false;
			int loop_len = num.length() >> 1;
			for(int j = 0; j < loop_len && !valid; j++)
			{
				if(num[j] != num[j + loop_len])
				{
					valid = true;
				}
			}
			if(!valid)
				sum += i;
		}
	}

	return sum;
}
