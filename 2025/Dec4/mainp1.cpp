#include <iostream>
#include <fstream>
#include <string>
#include <vector>

bool checkRoll(std::vector<std::string> *map, int x, int y);
int LINE_LEN;
int NUM_LINES;

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
	std::vector<std::string>* lines = new std::vector<std::string>;
	int sum = 0;

	while(std::getline(file, line))
	{
		lines->push_back(line);
	}

	NUM_LINES = lines->size();
	LINE_LEN = (*lines)[0].length();

	for(int i = 0; i < NUM_LINES; i++)
	{
		for(int j = 0; j < LINE_LEN; j++)
		{
			bool valid = checkRoll(lines, j, i);
			if(valid)
			{
				// std::cout << 'x';
				sum++;
			}
			// else
			// 	std::cout << (*lines)[i][j];
		}
		// std::cout << std::endl;
	}
	// std::cout << std::endl;

	std::cout << "Num Accessible = " << sum << std::endl;

	return 0;
}

bool checkRoll(std::vector<std::string> *map, int x, int y)
{
	int num_neighbors = 0;
	if((*map)[y][x] != '@')
		return false;
	//check all 8 directions
	//-1, -1
	if(x > 0 && y > 0 && (*map)[y -1][x -1] == '@')
		num_neighbors++;
	//0, -1
	if(y > 0 && (*map)[y - 1][x] == '@')
		num_neighbors++;
	//1, -1
	if(x < LINE_LEN - 1 && y > 0 && (*map)[y -1][x+1] == '@')
		num_neighbors++;
	//-1, 0
	if(x > 0 && (*map)[y][x - 1] == '@')
		num_neighbors++;
	//1, 0
	if(x < LINE_LEN - 1 && (*map)[y][x + 1] == '@')
		num_neighbors++;
	//-1, 1
	if(x > 0 && y < NUM_LINES - 1 && (*map)[y + 1][x - 1] == '@')
		num_neighbors++;
	//0, 1
	if(y < NUM_LINES - 1 && (*map)[y + 1][x] == '@')
		num_neighbors++;
	//1, 1
	if(x < LINE_LEN - 1 && y < NUM_LINES - 1 && (*map)[y + 1][x + 1] == '@')
		num_neighbors++;

	return (num_neighbors < 4) ? true : false;
}
