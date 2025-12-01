#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Block
{
	int64_t id;
	Block()
	{

	}
	Block(int64_t n_id)
	{
		id = n_id;
	}
	void disp()
	{
		char disp_char = (id == -1) ? '.' : (id % 10) + 0x30;
		std::cout << disp_char;
	}
};

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
	std::vector<Block> blocks;

	std::getline(file, line);

	for(int64_t i = 0; i < line.length(); i += 2)
	{
		int64_t id = i >> 1;
		int file_len = line[i] - 0x30;
		int free_len = line [i + 1] - 0x30;
		for(int j = 0; j < file_len; j++)
			blocks.push_back(Block(id));
		for(int j = 0; j < free_len; j++)
			blocks.push_back(Block(-1));
	}

	uint64_t num = 0;

	int64_t j = blocks.size() - 1;
	for(int64_t i = 0; i < blocks.size() && i < j - 1; i++)
	{
		if(blocks[i].id == -1)
		{
			while(blocks[j].id == -1)
				j--;
			/*std::cout << "(" << i << ", " << j << ")" << std::endl;*/
			blocks[i].id = blocks[j].id;
			blocks[j].id = -1;
		}
	}

	int64_t i = 0;
	for(Block block : blocks)
	{
		if(block.id != -1)
			num += i * block.id;
		i++;
	}

	std::cout << "Checksum = " << num << std::endl;

	return 0;
}

