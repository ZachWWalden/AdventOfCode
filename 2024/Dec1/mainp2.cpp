#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <new>
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

	std::vector<int64_t> left, right;
	std::vector<std::string>* split_string;
	int line_cnt =0;
	while(std::getline(file, line))
	{
		line_cnt++;
		split_string = splitString(line, ' ');
		if(split_string->size() != 2)
		{
			std::cout << "Error incorrect number of integers. Error on line: " << line_cnt << " of input." << std::endl;
			exit(-1);
		}
		for(int i = 0; i < 2; i++)
		{
			if(i == 0)
				left.push_back(std::stoi((*split_string)[i]));
			else
				right.push_back(std::stoi((*split_string)[i]));
		}
	}

	//sort the vectors
	std::sort(left.begin(), left.end(), [](uint64_t a, uint64_t b){ return a < b; });
	std::sort(right.begin(), right.end(), [](uint64_t a, uint64_t b){ return a < b; });

	if(left.size() != right.size())
	{
		std::cout << "Left and Right columns do not contain the same number of elements" << std::endl;
		exit(-1);
	}

	int64_t max =0;
	if(left[left.size() - 1] >= right[right.size() - 1])
		max = left[left.size() - 1];
	else
		max = right[right.size() - 1];

	std::cout << "Max = " << max << std::endl;

	int64_t *right_cnt = new int64_t[max];

	for(int k = 0; k <= max; k++)
	{
		right_cnt[k] = 0;
	}

	for(int k = 0; k < right.size(); k++)
	{
		right_cnt[right[k]]++;
	}

	int num = 0;

	for(int j = 0; j < left.size(); j++)
	{
		num += left[j] * right_cnt[left[j]];
	}

	delete [] right_cnt;

	std::cout << "Similarity Score = " << num << std::endl;

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
