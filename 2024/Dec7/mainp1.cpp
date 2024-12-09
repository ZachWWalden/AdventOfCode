#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Calibration
{
	uint64_t test_val;
	std::vector<uint64_t>* num_list;
	Calibration()
	{

	}
	Calibration(uint64_t n_test_val, std::vector<uint64_t>* n_num_list)
	{
		test_val = n_test_val;
		num_list = n_num_list;
	}
	void disp()
	{
		std::cout << test_val << ": ";
		for(int64_t num : *num_list)
		{
			std::cout << num << " ";
		}
		std::cout << std::endl;
	}
};

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

	std::vector<Calibration> data;
	std::vector<std::string>* split_string;

	while(std::getline(file, line))
	{
		/*std::cout << line << std::endl;*/
		split_string = splitString(line, ':');
		/*std::cout << "Split on :" << std::endl;*/
		uint64_t test_val = std::stoll((*split_string)[0]);
		/*std::cout << "Parse Test Val" << std::endl;*/
		auto tmp = split_string;
		split_string = splitString((*split_string)[1], ' ');
		/*std::cout << "Split on space" << std::endl;*/
		delete tmp;
		std::vector<uint64_t>* num_list = new std::vector<uint64_t>;
		for(int i = 0; i < split_string->size(); i++)
		{
			num_list->push_back(std::stoll((*split_string)[i]));
		}
		delete split_string;
		Calibration line(test_val, num_list);
		/*line.disp();*/
		data.push_back(line);
		/*std::cout << std::endl;*/
	}

	uint64_t num = 0;

	for(int i = 0; i < data.size(); i++)
	{
		bool calibrated = false;
		for(int k = 0; k < pow(2, data[i].num_list->size() - 1) && !calibrated; k++)
		{
			uint64_t val = data[i].num_list->at(0);
			for(int j = 0; j < data[i].num_list->size() - 1; j++)
			{
				int op = (k >> j) & 0x00000001;
				if(op == 1)
					val *= data[i].num_list->at(j + 1);
				else
					val += data[i].num_list->at(j + 1);
			}
			if(val == data[i].test_val)
				calibrated = true;
		}
		if(calibrated)
			num += data[i].test_val;
	}

	std::cout << "Total Distance = " << num << std::endl;

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
