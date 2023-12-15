#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <vector>


std::vector<std::string>* splitString(std::string string, char delimeter);
uint64_t hashAlgorithm(std::string string);

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
	std::vector<std::string>* lines;

	while(std::getline(file, line))
	{
		lines = splitString(line, ',');
	}
	uint64_t sum = 0;

	for(std::string str : *lines)
	{
		//std::cout << str << " ";
		uint64_t tmp = hashAlgorithm(str);
		//std::cout << " - " << tmp << std::endl;
		sum += tmp;
	}

	std::cout << "Sum = " << sum << std::endl;

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

uint64_t hashAlgorithm(std::string string)
{
	uint64_t sum = 0;
	for(int i = 0; i < string.length(); i++)
	{
		sum += string[i];
		sum *= 17;
		sum = sum % 256;
	}

	return sum;
}
