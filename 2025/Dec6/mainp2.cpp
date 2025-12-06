#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Operation
{
	std::vector<uint64_t>* operands;
	bool multiply;
	Operation(std::vector<uint64_t> *n_operands, bool n_multiply)
	{
		operands = n_operands;
		multiply = n_multiply;
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
	std::vector<std::string> lines;

	uint64_t sum = 0;

	while(std::getline(file, line))
	{
		lines.push_back(line);
	}

	//Operand character marks the 1st index of an operation block.
	//loop through each column
	std::vector<Operation> operations;
	std::vector<uint64_t> *cur_operands = new std::vector<uint64_t>;
	for(int i = lines[0].length() - 1; i >= 0; i--)
	{
		std::string cur_number = "";
		int j;
		for(j = 0; j < lines.size() - 1; j++)
		{
			if(lines[j][i] != ' ')
				cur_number += lines[j][i];
		}
		cur_operands->push_back(stoull(cur_number, nullptr, 10));
		//check last line for operand. if found, set operand. and reset temp data structure.
		if(lines[j][i] == '*' || lines[j][i] == '+')
		{
			operations.push_back(Operation(cur_operands, (lines[j][i] == '*' ? true : false)));
			cur_operands = new std::vector<uint64_t>;
			i--;
		}
		// std::cout << cur_number << std::endl;
	}

	for(Operation op : operations)
	{
		uint64_t val;
		if(op.multiply)
			val = 1;
		else
			val = 0;

		for(uint64_t operand : (*(op.operands)))
		{
			// std::cout << operand << std::endl;
			if(op.multiply)
				val *= operand;
			else
				val += operand;
		}
		// if(op.multiply)
			// std::cout << '*' << std::endl << std::endl;
		// else
			// std::cout << '+' << std::endl << std::endl;
		sum += val;
	}

	std::cout << "Total Sum = " << sum << std::endl;

	return 0;
}
