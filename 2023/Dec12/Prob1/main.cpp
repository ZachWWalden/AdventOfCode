#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>

struct Line
{
	int* nums;
	int size_nums;


};

int parseInt(std::string parseVal);
std::vector<std::string>* splitString(std::string string, char delimeter);

/*	Total Num Left = 2^(Num Question Mark)
 *
 *  num_damaged_remaining, num_contiguous_blocks, unkown_block_length, num_unkown_blocks_over_one, num_unkown, can_contig_unkown_fit
 *  2					   2					  3					   1						   3
 *
 *  2^3 -
 *
 * ?#?#?#?#?#?#?#? 1,3,1,6
 * .#.###.#.######
 *
 * ?###???????? 3,2,1
 * .###.??????? (3),2,1: last_block_length = num
 *
 *      ##.#...
 *      ##..#..
 *      ##...#.
 *      ##....#
 *      .##.#..
 *      .##..#.
 *      .##...#
 *      ..##.#.
 *      ..##..#
 *      ...##.#
 *
 *
 */

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

	while(std::getline(file, line))
	{
		lines.push_back(line);
	}

	return 0;
}

int parseInt(std::string parseVal)
{
	int mult = 0, digCnt = 0;
	int num = 0;

	for(int i = 0; i < parseVal.length(); i++)
	{
		if(parseVal[i] >= 0x30 && parseVal[i] <= 0x39)
		{
			mult = (digCnt > 0) ? 10 : 0;
			num = (num * mult) + (parseVal[i] - 0x30);
			digCnt++;
			//std::cout << "CHAR: " << parseVal[1] << ", NUM: " << num << std::endl;
		}
	}

	//std::cout << parseVal << ", LEN: " << parseVal.length() << ", VAL: " << num << std::endl;

	return num;
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
