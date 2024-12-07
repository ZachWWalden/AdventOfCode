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
	uint32_t x, y;
	uint32_t key, dkey;

	Point()
	{
	}
	Point(uint32_t n_x, uint32_t n_y, int dx, int dy)
	{
		x = n_x, y = n_y;
		uint32_t a = n_x + 1, b = n_y + 1, c = dx + 2, d = dy + 2;
		key = (a >= b) ? a * a + a + b : a + b * b;
		dkey = (c >= d) ? c * c + c +d : c + d * d;
	}
	void setPoint(uint32_t n_x, uint32_t n_y, int dx, int dy)
	{
		x = n_x, y = n_y;
		uint32_t a = n_x + 1, b = n_y + 1, c = dx + 2, d = dy + 2;
		key = (a >= b) ? a * a + a + b : a + b * b;
		dkey = (c >= d) ? c * c + c +d : c + d * d;
	}
	uint32_t getKey()
	{
		return key;
	}
	uint32_t getDkey()
	{
		return dkey;
	}
	bool operator==(Point p2)
	{
		return (x == p2.x && y == p2.y) ? true : false;
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
    const std::string BLUE("\033[34m");
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
	Point cur_point, next_point, start_point;

	for(int i = 0; i < data.size() && !found; i++)
	{
		for(int j = 0; j < data[i].length() && !found; j++)
		{
			if((data[i])[j] == '^')
			{
				dx = 0;
				dy = -1;
				start_point.setPoint(j, i, dy, dy);
				found = true;
			}
			else if((data[i])[j] == 'v')
			{
				dx = 0;
				dy = -1;
				start_point.setPoint(j, i, dy, dy);
				found = true;
			}
			else if((data[i])[j] == '<')
			{
				dx = 0;
				dy = -1;
				start_point.setPoint(j, i, dy, dy);
				found = true;
			}
			else if((data[i])[j] == '>')
			{
				dx = 0;
				dy = -1;
				start_point.setPoint(j, i, dy, dy);
				found = true;
			}
		}
	}

	int dx_tmp = dx, dy_tmp = dy;

	next_point = start_point;
	int cnt = 0;
	while(next_point.x >= 0 && next_point.y >= 0 && next_point.x < data[0].length() && next_point.y < data.size())
	{
		cur_point = next_point;
		//add cur point to map
		if(!(cur_point == start_point))
			visited_tiles[cur_point.key] = cur_point;
		//compute nex point
		next_point.setPoint(cur_point.x + dx, cur_point.y + dy, dx, dy);
		//egge case
		//
		//		.#.
		//		#<.
		//		.#.
		while(next_point.x >= 0 && next_point.y >= 0 && next_point.x < data[0].length() && next_point.y < data.size() && (data[next_point.y])[next_point.x] == '#')
		{
			turnRight(dx, dy);
			next_point.setPoint(cur_point.x + dx, cur_point.y + dy, dx, dy);
		}
	}

	int num = 0;

	std::unordered_map<uint32_t, Point> obs_points;
	for(auto point : visited_tiles)
	{
		cnt++;
		std::unordered_map<uint32_t, Point>* cycle_tiles = new std::unordered_map<uint32_t, Point>(8192);
		next_point = start_point;
		dx = dx_tmp;
		dy = dy_tmp;
		data[point.second.y][point.second.x] = '#';
		//add in obstacle
		bool cycle_found = false;
		while(next_point.x >= 0 && next_point.y >= 0 && next_point.x < data[0].length() && next_point.y < data.size() && !cycle_found)
		{
			cur_point = next_point;

			//add cur point to map
			(*cycle_tiles)[cur_point.key] = cur_point;
			//compute nex point
			next_point.setPoint(cur_point.x + dx, cur_point.y + dy, dx, dy);
			//egge case
			//
			//		.#.
			//		#<.
			//		.#.
			while(next_point.x >= 0 && next_point.y >= 0 && next_point.x < data[0].length() && next_point.y < data.size() && (data[next_point.y])[next_point.x] == '#')
			{
				turnRight(dx, dy);
				next_point.setPoint(cur_point.x + dx, cur_point.y + dy, dx, dy);
			}
			if(cycle_tiles->find(next_point.key) != cycle_tiles->end() && (*(cycle_tiles->find(next_point.key))).second.dkey == next_point.dkey)
				cycle_found = true;
		}

		if(cycle_found)
		{
			obs_points[point.first] = point.second;
			num++;
		}
		/*
		for(int i = 0; i < data.size(); i++)
		{
			for(int j = 0; j < data[i].length(); j++)
			{
				if(cycle_tiles->find(Point(j, i, 0, 0).getKey()) != cycle_tiles->end())
					std::cout << GREEN << (data[i])[j] << RESET;
				else if(point.second == Point(j, i, 0, 0))
					std::cout << BLUE << (data[i])[j] << RESET;
				else
					std::cout << RED << (data[i])[j] << RESET;
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		*/

		data[point.second.y][point.second.x] = '.';
		delete cycle_tiles;
	}

	for(int i = 0; i < data.size(); i++)
	{
		for(int j = 0; j < data[i].length(); j++)
		{
			if(obs_points.find(Point(j, i, 0, 0).getKey()) != obs_points.end())
				std::cout << BLUE << (data[i])[j] << RESET;
			else
				std::cout << RED << (data[i])[j] << RESET;
		}
		std::cout << std::endl;
	}

	std::cout << "Num Tiles = " << cnt << std::endl;
	std::cout << "Number of Obstructable tiles = " << num << std::endl;

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
