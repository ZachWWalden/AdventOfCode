#include <cstdint>
#include <cstdlib>
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

struct File
{
	int64_t id;
	uint64_t start_block;
	uint64_t num_blocks;
	File()
	{

	}
	File(int64_t n_id, uint64_t n_start_block, uint64_t n_num_blocks)
	{
		id = n_id;
		start_block = n_start_block;
		num_blocks = n_num_blocks;
	}
	void disp()
	{
		std::cout << "ID = " << id << ", Start Block = " << start_block << ", Length = " << num_blocks << std::endl;
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
	std::vector<File> files;

	std::getline(file, line);

	uint64_t cur_block = 0;
	for(int64_t i = 0; i < line.length(); i += 2)
	{
		int64_t id = i >> 1;
		uint64_t file_len = line[i] - 0x30;
		files.push_back(File(id, cur_block, file_len));
		cur_block += file_len;
		for(int j = 0; j < file_len; j++)
			blocks.push_back(Block(id));
		if((i + 1) < line.length())
		{
			int64_t free_len = line [i + 1] - 0x30;
			files.push_back(File(-1, cur_block, free_len));
			cur_block += free_len;
			for(int j = 0; j < free_len; j++)
				blocks.push_back(Block(-1));
		}
	}

	for(File file: files)
	{
		file.disp();
	}

	uint64_t num = 0;

	int64_t j = files.size() - 1;
	for(int64_t i = 0; i < files.size() && i < j - 1; i++)
	{
		if(files[i].id == -1)
		{
			while(files[j].id == -1)
				j--

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

