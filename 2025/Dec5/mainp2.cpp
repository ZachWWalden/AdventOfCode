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
	bool touched = false;
};

std::vector<std::string>* splitString(std::string string, char delimeter);
Range parseRange(std::string range_str);
bool compRange(Range a, Range b);
Range combineRanges(Range a, Range b);
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
	std::vector<Range> combined_rngs;
	uint64_t sum = 0;

	std::getline(file, line);
	while(line != "")
	{
		rngs.push_back(parseRange(line));
		std::getline(file, line);
	}

	//sort range vector;
	std::sort(rngs.begin(), rngs.end(), compRange);

	std::vector<Range>::iterator inner, outer;
	//combine ranges;
	for(outer = rngs.begin(); outer < rngs.end(); outer++)
	{
		Range cur_base_rng = (*outer);
		for(inner = outer + 1; inner < rngs.end() && !cur_base_rng.touched; inner++)
		{
			//if combinable && inner !touched -> combine
			if(isInRange(cur_base_rng, (*inner).start) && !((*inner).touched))
			{
				cur_base_rng = combineRanges(cur_base_rng, (*inner));
				(*inner).touched = true;
			}
		}
		if(!cur_base_rng.touched)
			combined_rngs.push_back(cur_base_rng);
	}

	for(Range rng : combined_rngs)
	{
		// std::cout << rng.start << "-" << rng.end << std::endl;
		sum += rng.end - rng.start + 1;
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

bool compRange(Range a, Range b)
{
	return a.start < b.start;
}

Range combineRanges(Range a, Range b)
{
	Range ret_val;
	ret_val.start = (a.start < b.start) ? a.start : b.start;
	ret_val.end = (b.end > a.end) ? b.end : a.end;
	ret_val.touched = false;
	return ret_val;
}

bool isInRange(Range rng, uint64_t id)
{
	return (id >= rng.start && id <= rng.end) ? true : false;
}
