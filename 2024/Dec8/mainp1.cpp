#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <unordered_map>

#define MAX_FREQS 62

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

int alphanumericToLinearIndex(const char character);
bool isInBounds(int x, int y, int max_x, int max_y);

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
	std::unordered_map<uint32_t, Point> antinodes(8192);
	std::vector<Point>* antennas[62];
	for(int i = 0; i < MAX_FREQS; i++)
	{
		antennas[i] = new std::vector<Point>;
	}

	while(std::getline(file, line))
	{
		data.push_back(line);
	}

	for(int i = 0; i < data.size(); i++)
	{
		for(int j = 0; j < data[i].length(); j++)
		{
			if((data[i])[j] != '.')
			{
				//antenna found.
				int idx = alphanumericToLinearIndex((data[i])[j]);
				antennas[idx]->push_back(Point(j, i));
				std::cout << BLUE << (data[i])[j] << RESET;
			}
			else
				std::cout << RED << (data[i])[j] << RESET;
		}
		std::cout << std::endl;
	}

	int num = 0;

	//loop through freqs
	for(int i = 0; i < MAX_FREQS; i++)
	{
		//loop though antennas at specific frequency pairwise
		for(int j = 0; antennas[i] ->size() != 0 && j < antennas[i]->size() - 1; j++)
		{
			Point p1 = antennas[i]->at(j);
			for(int k = j + 1; k < antennas[i]->size(); k++)
			{
				Point p2 = antennas[i]->at(k);
				int dx = p2.x - p1.x;
				int dy = p2.y - p1.y;
				//p2 + d(x/y)
				int an1_x = p2.x + dx, an1_y = p2.y + dy;
				if(isInBounds(an1_x, an1_y, data[0].length(), data.size()))
				{
					Point an1(an1_x, an1_y);
					antinodes[an1.key] = an1;
				}
				//p1 - d(x/y)
				int an2_x = p1.x - dx, an2_y = p1.y - dy;
				if(isInBounds(an2_x, an2_y, data[0].length(), data.size()))
				{
					Point an2(an2_x, an2_y);
					antinodes[an2.key] = an2;
				}
			}
		}
	}

	for(auto point : antinodes)
	{
		num++;
	}

	std::cout << std::endl;

	for(int i = 0; i < data.size(); i++)
	{
		for(int j = 0; j < data[i].length(); j++)
		{
			if(antinodes.find(Point(j, i).key) != antinodes.end())
				std::cout << BLUE << "#" << RESET;
			else
				std::cout << RED << (data[i])[j] << RESET;
		}
		std::cout << std::endl;
	}


	std::cout << "Number of antinodes = " << num << std::endl;

	return 0;
}


int alphanumericToLinearIndex(const char character)
{
	//digits 0x30-0x39
	if(character >= 0x30 && character <= 0x39)
		return character - 0x30;
	//uppercase 0x41-0x5A
	else if(character >= 0x41 && character <= 0x5A)
		return character - 0x37;
	//lowercase 0x61-0x7A
	else if(character >= 0x61 && character <= 0x7A)
		return character - 0x3D;
	else
		return -1;
}

bool isInBounds(int x, int y, int max_x, int max_y)
{
	return (x >= 0 && x < max_x && y >= 0 && y < max_y);
}
