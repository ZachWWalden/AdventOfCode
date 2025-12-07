#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>

struct Point
{
	int x, y;
	uint64_t sum;
	bool split;
	Point(int n_x, int n_y, bool n_split, uint64_t n_sum)
	{
		x = n_x;
		y = n_y;
		split = n_split;
		sum = n_sum;
	}
	bool isEqual(Point pt)
	{
		return (x == pt.x && y == pt.y);
	}
};

bool isPointInList(std::vector<Point> *list, Point &pt);

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
	std::vector<Point>* cur_row_pts = new std::vector<Point>;
	std::vector<Point>* nxt_row_pts = new std::vector<Point>;
	for(int i = 0; i < lines[0].length(); i++)
	{
		// std::cout << lines[0][i];
		if(lines[0][i] == 'S')
		{
			cur_row_pts->push_back(Point(i, 0, false, 1));
			break;
		}
	}

	//loop through all the rows of points
	for(int i = 0; i < lines.size() - 1; i++)
	{
		nxt_row_pts = new std::vector<Point>;
		//loop through points in a row.
		for(Point pt : *(cur_row_pts))
		{
			if(lines[pt.y + 1][pt.x] == '^')
			{
				//split the beam.
				Point left(pt.x - 1, pt.y + 1, true, pt.sum);
				Point right(pt.x + 1, pt.y + 1, true, pt.sum);
				if(!isPointInList(nxt_row_pts, left))
					nxt_row_pts->push_back(left);
				if(!isPointInList(nxt_row_pts, right))
					nxt_row_pts->push_back(right);
			}
			else
			{
				Point down(pt.x, pt.y + 1, false, pt.sum);
				if(!isPointInList(nxt_row_pts, down))
					nxt_row_pts->push_back(down);
			}
		}
		delete cur_row_pts;
		cur_row_pts = nxt_row_pts;
	}

	for(Point pt : *cur_row_pts)
		sum += pt.sum;

	std::cout << "Total Timelines = " << sum << std::endl;

	return 0;
}

bool isPointInList(std::vector<Point> *list, Point &pt)
{
	for(int i = 0; i < list->size(); i++)
	{
		if((*list)[i].isEqual(pt))
		{
			(*list)[i].sum += pt.sum;
			// std::cout << "Dupe, Sum = " << row_pt.sum << std::endl;
			return true;
		}
	}
	return false;
}
