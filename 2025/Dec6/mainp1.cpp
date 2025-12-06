#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::string>* splitString(std::string string, char delimeter);

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
	std::vector<std::vector<std::string>*> lines;

	uint64_t sum = 0;

	while(std::getline(file, line))
	{
		lines.push_back(splitString(line, ' '));
	}

	int NUM_LINES = lines.size() - 1;
	int LINE_LEN = lines[0]->size();
	//loop through a line
	for(int i = 0; i < LINE_LEN; i++)
	{
		// array of operands
		std::vector<uint64_t> operands;
		//loop through lines, except the last.
		int j;
		for(j = 0; j < NUM_LINES; j++)
		{
			//add each operand to array
			// std::cout << (*(lines[j]))[i] << std::endl;
			operands.push_back(stoull((*(lines[j]))[i], nullptr, 10));
		}
		//get operator from last element
		bool multiply = false;
		// std::cout << (*(lines[j]))[i] << std::endl;
		if((*lines[j])[i] == "*")
		{
			// std::cout << "Multiply" << std::endl;
			multiply = true;
		}

		uint64_t val;
		if(multiply)
			val = 1;
		else
			val = 0;
		//loop through operand array
		for(uint64_t operand : operands)
		{
			if(multiply)
				val *= operand;
			else
				val += operand;
			// std::cout << "Val = " << val << std::endl;
		}
		// std::cout << std::endl;
		sum += val;
	}


	std::cout << "Total Sum = " << sum << std::endl;

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

