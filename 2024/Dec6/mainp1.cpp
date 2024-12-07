#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <unordered_map>

struct Point
{
	uint32_t x, y, key;

	Point()
	{
	}
	Point(uint32_t n_x, uint32_t n_y)
	{
		x = n_x, y = n_y;
		uint32_t a = n_x + 1, b = n_y + 1;
		key = (a >= b) ? a * a + a + b : a + b * b;
	}
	void setPoint(uint32_t n_x, uint32_t n_y)
	{
		x = n_x, y = n_y;
		uint32_t a = n_x + 1, b = n_y + 1;
		key = (a >= b) ? a * a + a + b : a + b * b;
	}
	uint32_t getKey()
	{
		return key;
	}
	void disp()
	{
		std::cout << "(" << x << ", " << y << ")" << std::endl;
	}
};

void turnRight(int &dx, int &dy);

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
	std::unordered_map<uint32_t, Point> visited_tiles(8192);
	while(std::getline(file, line))
	{
		data.push_back(line);
	}

	int dx = 0, dy = 0;
	bool found = false;
	Point cur_point, next_point(0,0);

	for(int i = 0; i < data.size() && !found; i++)
	{
		for(int j = 0; j < data[i].length() && !found; j++)
		{
			if((data[i])[j] == '^')
			{
				cur_point.setPoint(j, i);
				dx = 0;
				dy = -1;
				found = true;
			}
			else if((data[i])[j] == 'v')
			{
				cur_point.setPoint(j, i);
				dx = 0;
				dy = -1;
				found = true;
			}
			else if((data[i])[j] == '<')
			{
				cur_point.setPoint(j, i);
				dx = 0;
				dy = -1;
				found = true;
			}
			else if((data[i])[j] == '>')
			{
				cur_point.setPoint(j, i);
				dx = 0;
				dy = -1;
				found = true;
			}
		}
	}

	next_point = cur_point;
	while(next_point.x >= 0 && next_point.y >= 0 && next_point.x < data[0].length() && next_point.y < data.size())
	{
		cur_point = next_point;
		//add cur point to map
		visited_tiles[cur_point.key] = cur_point;
		//compute nex point
		next_point.setPoint(cur_point.x + dx, cur_point.y + dy);
		//egge case
		//
		//		.#.
		//		#<.
		//		.#.
		while(next_point.x >= 0 && next_point.y >= 0 && next_point.x < data[0].length() && next_point.y < data.size() && (data[next_point.y])[next_point.x] == '#')
		{
			turnRight(dx, dy);
			next_point.setPoint(cur_point.x + dx, cur_point.y + dy);
		}
	}

	int num = 0;

	for(auto point : visited_tiles)
	{
		num++;
	}

	std::cout << "Number of tiles visited by guard = " << num << std::endl;

	return 0;
}

void turnRight(int &dx, int &dy)
{
	if(dy == 0)
	{
		if(dx == 1)
		{
			dx = 0; //dx = dy
			dy = 1; //dy = dx
		}
		//dx is -1
		else
		{
			dx = 0; //dx = dy
			dy = -1; //dy = dx
		}
	}
	else if(dx == 0)
	{
		if(dy == 1)
		{
			dy = 0; //dy = dx
			dx = -1; //dx = -1 * dy
		}
		//dy is -1
		else
		{
			dy = 0; //dy = dx
			dx = 1; //dy = -dx
		}
	}
}
