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
	Vec2I( 1, -1),
	Vec2I(-1,  1),
	Vec2I( 1,  1)
};

int validateMatch(std::vector<std::string> &data, Vec2I location);

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
			j = data[i].find("A", j);
			if(j != std::string::npos)
			{
				/*std::cout << "Found X at (" << j << ", " << i << "), ";*/
				int tmp = num;
				//check all 8 directions for a match;
				num += validateMatch(data, Vec2I(j, i));

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

int validateMatch(std::vector<std::string> &data, Vec2I location)
{
	int ret_val = 0;
	char chars[2][2];

	bool valid = true;
	for(int i = 0; i < 4 && valid; i++)
	{
		Vec2I point = location + directions[i];
		//Check for out of bounds, assumes each line has the same length. This is true for the given input
		if(point.x < 0 || point.x >= data[0].length() || point.y < 0 || point.y >= data.size() || !(data[point.y][point.x] == 'M' || data[point.y][point.x] == 'S'))
		{
			valid = false;
		}
		else
		{
			//we have a potential point
			if(i == 0)
				chars[0][0] = data[point.y][point.x];
			else if(i == 1)
				chars[0][1] = data[point.y][point.x];
			else if(i == 2)
				chars[1][0] = data[point.y][point.x];
			else
				chars[1][1] = data[point.y][point.x];
		}
	}
	//4 possibilities:
	//					M M    S M    S S    M S
	//					S S    S M    M M    M S

	if(chars[0][0] == 'M' && chars[0][1] == 'M' && chars[1][0] == 'S' && chars[1][1] == 'S')
		valid = true;
	else if(chars[0][0] == 'S' && chars[0][1] == 'M' && chars[1][0] == 'S' && chars[1][1] == 'M')
		valid = true;
	else if(chars[0][0] == 'S' && chars[0][1] == 'S' && chars[1][0] == 'M' && chars[1][1] == 'M')
		valid = true;
	else if(chars[0][0] == 'M' && chars[0][1] == 'S' && chars[1][0] == 'M' && chars[1][1] == 'S')
		valid = true;
	else
		valid = false;

	if(valid)
	{
		ret_val = 1;
		valid_pnts.push_back(location);
		for(int i = 0; i < 4; i++)
		{
			valid_pnts.push_back(location + directions[i]);
		}
	}

	return ret_val;
}
