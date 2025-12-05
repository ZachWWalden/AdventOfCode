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
uint64_t parseId(std::string id_str);
bool isInRange(Range rng, uint64_t id);

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

	std::vector<Range> rngs;
	std::vector<uint64_t> ids;
	uint64_t sum = 0;

	std::getline(file, line);
	while(line != "")
	{
		rngs.push_back(parseRange(line));
		std::getline(file, line);
	}

	while(std::getline(file, line))
	{
		ids.push_back(parseId(line));
	}

	// for(Range rng : rngs)
	// {
	// 	std::cout << rng.start << "-" << rng.end << std::endl;
	// }
	// std::cout << std::endl;
	// for(uint64_t id : ids)
	// {
	// 	std::cout << id << std::endl;
	// }
	// std::cout << std::endl;

	for(uint64_t id : ids)
	{
		bool fresh = false;
		for(Range rng : rngs)
		{
			if(isInRange(rng, id) && !fresh)
			{
				// std::cout << "Fresh " << id << std::endl;
				fresh = true;
				sum++;
			}
		}
	}

	std::cout << "Num Fresh = " << sum << std::endl;

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

uint64_t parseId(std::string id_str)
{
	return std::stoull(id_str, nullptr, 10);
}

bool isInRange(Range rng, uint64_t id)
{
	return (id >= rng.start && id <= rng.end) ? true : false;
}
