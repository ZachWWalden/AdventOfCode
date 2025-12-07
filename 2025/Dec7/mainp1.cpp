#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Point
{
	int x, y;
	bool split;
	Point(int n_x, int n_y, bool n_split)
	{
		x = n_x;
		y = n_y;
		split = n_split;
	}
	bool isEqual(Point pt)
	{
		return (x == pt.x && y == pt.y);
	}
};

bool isPointInList(std::vector<Point> *list, Point pt);

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
		for(int i = 0; i < line.length(); i++)
		{
			if(line[i] == 'S' || line[i] == '^')
			{
				lines.push_back(line);
				break;
			}
		}
	}
	//list containing all points with a beam only once, separated by row.
	std::vector<std::vector<Point>*> beam_pts;
	std::vector<Point>* row_pts = new std::vector<Point>;
	for(int i = 0; i < lines[0].length(); i++)
	{
		// std::cout << lines[0][i];
		if(lines[0][i] == 'S')
		{
			row_pts->push_back(Point(i, 0, false));
			break;
		}
	}
	beam_pts.push_back(row_pts);

	//loop through all the rows of points
	for(int i = 0; i < lines.size() - 1; i++)
	{
		row_pts = new std::vector<Point>;
		//loop through points in a row.
		for(Point pt : *(beam_pts[i]))
		{
			if(lines[pt.y + 1][pt.x] == '^')
			{
				//split the beam.
				Point left(pt.x - 1, pt.y + 1, true);
				Point right(pt.x + 1, pt.y + 1, true);
				if(!isPointInList(row_pts, left))
					row_pts->push_back(left);
				if(!isPointInList(row_pts, right))
					row_pts->push_back(right);
				sum++;
			}
			else
			{
				Point down(pt.x, pt.y + 1, false);
				if(!isPointInList(row_pts, down))
					row_pts->push_back(down);
			}
		}
		beam_pts.push_back(row_pts);
	}

	std::cout << "Total Splits = " << sum << std::endl;

	return 0;
}

bool isPointInList(std::vector<Point> *list, Point pt)
{
	for(Point row_pt : *list)
	{
		if(row_pt.isEqual(pt))
			return true;
	}
	return false;
}
