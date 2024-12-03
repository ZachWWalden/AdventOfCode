#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define MAX_INST_LEN 14

struct MatchDictionary
{
	std::string mul_str = "mul(";
	std::string do_str = "do()";
	std::string dont_str = "don't()";
	MatchDictionary()
	{

	}
};

struct Vec2I
{
	uint64_t a,b;
	Vec2I()
	{

	}
	Vec2I(uint64_t n_a, uint64_t n_b)
	{
		a = n_a;
		b = n_b;
	}
	uint64_t getMultVal()
	{
		return a*b;
	}
	void disp()
	{
		std::cout << "A = " << a << ", B = " << b << std::endl;
	}
};

std::vector<std::string>* splitString(std::string string, char delimeter);
std::vector<Vec2I>* parseData(std::vector<std::string> &data);
Vec2I parseAndValidateMulSubStr(std::string str, bool &valid);

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

	std::vector<std::string> data;
	while(std::getline(file, line))
	{
		data.push_back(line);
	}

	/*std::cout << "Parsing Data" << std::endl;*/
	std::vector<Vec2I>* valid_instructions = parseData(data);
	/*std::cout << "Parsed Data" << std::endl;*/
	uint64_t num = 0;

	for(int i = 0; i < valid_instructions->size(); i++)
	{
		num += (*valid_instructions)[i].getMultVal();
		/*(*valid_instructions)[i].disp();*/
	}

	std::cout << "Multiplication Sum = " << num << std::endl;

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
std::vector<Vec2I>* parseData(std::vector<std::string> &data)
{
	MatchDictionary matches;
	std::vector<Vec2I>* ret_val = new std::vector<Vec2I>;
	bool enabled = true;

	for(int i = 0; i < data.size(); i++)
	{
		int j = 0;
		std::size_t pos = 0, dont_pos = 0, do_pos = 0;

		/*std::cout << data[i] << std::endl;*/

		dont_pos = data[i].find(matches.dont_str, j);
		if(dont_pos == std::string::npos)
			dont_pos = data[i].length();
		//std::cout << "Dont Pos = " << dont_pos << std::endl;

		while(j < data[i].length() && pos != std::string::npos)
		{
			if(enabled && j < dont_pos)
			{
				//search for the substring
				bool valid = false;
				pos = data[i].find(matches.mul_str, j);
				if(pos != std::string::npos && pos < dont_pos)
				{
					//found first match
					Vec2I inst = parseAndValidateMulSubStr(data[i].substr(pos, MAX_INST_LEN), valid);
					if(valid)
					{
						ret_val->push_back(inst);
					}
					j = pos + 1;
				}
				else
					j = dont_pos;
			}
			else if(enabled && j >= dont_pos)
			{
				/*std::cout << "dont pos = " << dont_pos << ", J = " << j << std::endl;*/
				dont_pos = data[i].find(matches.dont_str, j);
				if(dont_pos == std::string::npos)
					dont_pos = data[i].length();
				enabled = false;
			}
			else
			{
				/*std::cout << "do pos = " << do_pos << ", J = " << j << std::endl;*/
				do_pos = data[i].find(matches.do_str, j);
				if(do_pos != std::string::npos)
				{
					j = do_pos;
					enabled = true;
				}
				else
				{
					j = data[i].length();
				}
			}
		}
	}

	return ret_val;
}

Vec2I parseAndValidateMulSubStr(std::string str, bool &valid)
{


	Vec2I ret_val;
	ret_val.a = -1;
	ret_val.b = -1;

	std::string dig1 = "", dig2 = "";

	bool good = true;
	bool done = false;
	int digit_cnt = 0, num_cnt = 0, comma_cnt = 0;
	for(int i = 4; i < str.length() && good && !done; i++)
	{
		/*std::cout << "str[i] = " << str[i] << std::endl;*/
		if(std::isdigit(str[i]) != 0)
		{
			digit_cnt++;
			if(num_cnt == 0)
				dig1 += str[i];
			else if(num_cnt == 1)
				dig2 += str[i];
		}
		else if(str[i] == ',')
		{
			if(digit_cnt < 1 || digit_cnt > 3 || num_cnt > 1)
			{
				good = false;
			}
			else
			{
				digit_cnt = 0;
				num_cnt++;
			}
			comma_cnt++;
		}
		else if(str[i] == ')')
		{
			if(digit_cnt > 0 && digit_cnt < 4)
				num_cnt++;
			if(digit_cnt > 3 || num_cnt != 2 || comma_cnt != 1)
				good = false;
			else
				done = true;
		}
		else
		{
			good = false;
		}
		/*std::cout << "Digit Count = " << digit_cnt << ", Num Count = " << num_cnt << ", Comma Count = " << comma_cnt << std::endl;*/
	}
	if(!good)
	{
		//std::cout << "Bad" << std::endl;
		valid = false;
	}
	else
	{
		/*std::cout << str << std::endl;*/
		valid = true;
		ret_val.a = std::stoi(dig1);
		ret_val.b = std::stoi(dig2);
		//ret_val.disp();
	}

	return ret_val;
}
