#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Vec2I
{
	int x,y;
	Vec2I()
	{

	}
	Vec2I(int n_x, int n_y)
	{
		x = n_x;
		y = n_y;
	}
	Vec2I operator+(Vec2I a)
	{
		return Vec2I(x + a.x, y + a.y);
	}
};

std::vector<Vec2I> valid_pnts;

Vec2I directions[8]= {
	Vec2I(-1, -1),
	Vec2I( 0, -1),
	Vec2I( 1, -1),
	Vec2I(-1,  0),
	Vec2I( 1,  0),
	Vec2I(-1,  1),
	Vec2I( 0,  1),
	Vec2I( 1,  1)
};

int validateMatch(std::vector<std::string> &data, Vec2I location, Vec2I direction);

int main(int argc, char* argv[])
{
	// Define color codes
    const std::string RED("\033[31m");
    const std::string GREEN("\033[32m");
    const std::string RESET("\033[0m");

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

	int num = 0;

	for(int i = 0; i < data.size(); i++)
	{
		int j = 0;
		while(j < data[i].length() && j != std::string::npos)
		{
			j = data[i].find("X", j);
			if(j != std::string::npos)
			{
				/*std::cout << "Found X at (" << j << ", " << i << "), ";*/
				int tmp = num;
				//check all 8 directions for a match;
				for(int k = 0; k < 8; k++)
				{
					num += validateMatch(data, Vec2I(j, i), directions[k]);
				}

				/*if(num != tmp)*/
				/*	std::cout << "Valid" << std::endl;*/
				/*else*/
				/*	std::cout << std::endl;*/

				j++;
			}
		}
	}

	for(int rows = 0; rows < data.size(); rows++)
	{
		for(int cols = 0; cols < data[rows].length(); cols++)
		{
			bool valid = false;
			for(int i = 0; i < valid_pnts.size() && !valid; i++)
			{
				if(cols == valid_pnts[i].x && rows == valid_pnts[i].y)
					valid = true;
			}
			if(valid)
				std::cout << GREEN << (data[rows])[cols] << RESET;
			else
				std::cout << RED << (data[rows])[cols] << RESET;

		}
		std::cout << std::endl;
	}

	std::cout << std::endl;

	std::cout << "Number of XMAS's = " << num << std::endl;

	return 0;
}

int validateMatch(std::vector<std::string> &data, Vec2I location, Vec2I direction)
{
	int ret_val = 0;
	char match_chars[3] = {'M', 'A', 'S'};
	Vec2I start = location;

	bool valid = true;
	for(int i = 0; i < 3 && valid; i++)
	{
		location = location + direction;
		//Check for out of bounds, assumes each line has the same length. This is true for the given input
		if(location.x < 0 || location.x >= data[0].length() || location.y < 0 || location.y >= data.size())
		{
			valid = false;
		}
		else
		{
			//location is not valid
			if((data[location.y])[location.x] != match_chars[i])
			{
				valid = false;
			}
		}
	}
	if(valid)
	{
		ret_val = 1;
		valid_pnts.push_back(start);
		for(int i = 0; i < 3; i++)
		{
			start = start + direction;
			valid_pnts.push_back(start);
		}
	}

	return ret_val;
}
