#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::string>* splitString(std::string string, char delimeter);
std::vector<int64_t>* parseReactorReport(std::vector<std::string>* split_line);

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

	std::vector<std::vector<int64_t>*> data;
	std::vector<std::string>* split_line;
	while(std::getline(file, line))
	{
		split_line = splitString(line, ' ');
		data.push_back(parseReactorReport(split_line));
		//delete the current line
		delete split_line;
	}

	int num = 0;

	//loop through data
	for(int i = 0; i < data.size(); i++)\
	{
		//loop through individual report for parsing
		bool safe = true, inc = false, dec = false;
		for(int j = 0; j < (*(data[i])).size() - 1 && safe; j++)
		{
			int64_t diff = (*(data[i]))[j] - (*(data[i]))[j + 1];
			//std::cout << "diff = " << diff << std::endl;
			int64_t abs_diff = std::abs(diff);
			if(abs_diff > 3 || abs_diff == 0)
			{
				safe = false;
				//report is not safe, no need to check the rest.
				break;
			}
			//decreasing
			if(diff > 0)
			{
				dec = true;
				if(inc)
					safe = false;
			}
			//increasing
			else
			{
				inc = true;
				if(dec)
					safe = false;
			}
		}
		if(safe)
		{
			num++;
			//std::cout << "Safe" << std::endl;
		}
		//std::cout << std::endl;
	}

	std::cout << "Total Safe = " << num << std::endl;

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

std::vector<int64_t>* parseReactorReport(std::vector<std::string>* split_line)
{
	std::vector<int64_t> *report = new std::vector<int64_t>;

	for(int i = 0; i < split_line->size(); i++)
	{
		report->push_back(std::stoi((*split_line)[i]));

	}
	return report;
}
