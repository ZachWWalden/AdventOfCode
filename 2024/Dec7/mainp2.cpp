#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

struct Number
{
	uint64_t num;
	uint8_t num_dig;
	Number()
	{

	}
	Number(uint64_t n_num, uint8_t n_num_dig)
	{
		num = n_num;
		num_dig = n_num_dig;
	}
	void disp()
	{
		std::cout << num << " ";
	}
};

struct Calibration
{
	uint64_t test_val;
	std::vector<Number>* num_list;
	Calibration()
	{

	}
	Calibration(uint64_t n_test_val, std::vector<Number>* n_num_list)
	{
		test_val = n_test_val;
		num_list = n_num_list;
	}
	void disp()
	{
		std::cout << test_val << ": ";
		for(Number num : *num_list)
		{
			num.disp();
		}
		std::cout << std::endl;
	}
};

std::vector<std::string>* splitString(std::string string, char delimeter);
void generatePermutations(int n, std::string str, std::vector<std::string>* vec);

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

	std::vector<Calibration> data, bad;
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
		std::vector<Number>* num_list = new std::vector<Number>;
		for(int i = 0; i < split_string->size(); i++)
		{
			std::string num = (*split_string)[i];
			num_list->push_back(Number(std::stoll(num), num.length()));
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
			uint64_t val = data[i].num_list->at(0).num;
			for(int j = 0; j < data[i].num_list->size() - 1; j++)
			{
				int op = (k >> j) & 0x00000001;
				if(op == 1)
					val *= data[i].num_list->at(j + 1).num;
				else
					val += data[i].num_list->at(j + 1).num;
			}
			if(val == data[i].test_val)
				calibrated = true;
		}
		if(calibrated)
			num += data[i].test_val;
		else
			bad.push_back(data[i]);
	}

	std::unordered_map<int, std::vector<std::string>*> cache(32);

	for(int i = 0; i < bad.size(); i++)
	{
		/*bad[i].disp();*/
		std::vector<std::string>* perms;
		if(cache.find(bad[i].num_list->size() - 1) != cache.end())
		{
			perms = cache[bad[i].num_list->size() - 1];
		}
		else
		{
			perms = new std::vector<std::string>;
			generatePermutations(bad[i].num_list->size() - 1, "", perms);
			cache[bad[i].num_list->size() - 1] = perms;
		}
		bool calibrated = false;
		for(int j = 0; j < perms->size() && !calibrated; j++)
		{
			/*std::cout << perms->at(j) << std::endl;*/
			uint64_t val = bad[i].num_list->at(0).num;
			for(int k = 0; k < perms->at(j).length(); k++)
			{
				if(perms->at(j)[k] == '*')
				{
					val *= bad[i].num_list->at(k + 1).num;
				}
				else if (perms->at(j)[k] == '+')
				{
					val += bad[i].num_list->at(k + 1).num;
				}
				else
				{
					val = std::stoll(std::to_string(val) + std::to_string(bad[i].num_list->at(k + 1).num));
					/*std::cout << "val = " << val << std::endl;*/
				}
			}
			if(val == bad[i].test_val)
				calibrated = true;
			/*std::cout << "End val = " << val << std::endl;*/
		}
		if(calibrated)
			num += bad[i].test_val;
		/*std::cout << std::endl;*/
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

void generatePermutations(int n, std::string str, std::vector<std::string>* vec)
{
	if(n != 0)
	{
		generatePermutations(n - 1,str + "*", vec);
		generatePermutations(n - 1,str + "+", vec);
		generatePermutations(n - 1,str + "|", vec);
	}
	else
	{
		vec->push_back(str);
	}
	return;
}

