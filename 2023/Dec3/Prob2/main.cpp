#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Point
{
	int x, y;
	Point(int nX, int nY)
	{
		x = nX;
		y = nY;
	}
	Point()
	{

	}
	void disp()
	{
		std::cout << "X: " << x << ", Y: " << y << std::endl;
	}
	bool isEqual(Point point)
	{
		if(point.x == x && point.y == y)
			return true;
		return false;
	}
	bool isAdjacent(Point point)
	{
		//8 possible isAdjacent points.
		if(point.x == x - 1 && point.y == y - 1)
			return true;
		else if(point.x == x && point.y == y - 1)
			return true;
		else if(point.x == x + 1 && point.y == y - 1)
			return true;
		else if(point.x == x - 1 && point.y == y)
			return true;
		else if(point.x == x + 1 && point.y == y)
			return true;
		else if(point.x == x - 1 && point.y == y + 1)
			return true;
		else if(point.x == x && point.y == y + 1)
			return true;
		else if(point.x == x + 1 && point.y == y + 1)
			return true;
		return false;
	}
};
struct Number
{
	int number, length;
	Point start;
	bool valid = false;
	Number(int nNum, Point nStart, int nLength)
	{
		number = nNum;
		start = nStart;
		length = nLength;
	}
	Number()
	{

	}
	void disp()
	{
		std::cout << "NUM: " << number << ", LEN: " << length << std::endl;
		start.disp();
	}
};

bool isSymbol(char chr);

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		std::cout << "crap" << std::endl;
		return -1;
	}

	std::string filename = argv[1];

	std::fstream file;

	file.open(filename, std::ios::in);

	int length = 0;//for
	std::vector<std::string> lines;
	std::vector<Number> nums;
	std::vector<Point> points;

	std::string line;

	while(std::getline(file, line))
	{
		length = line.length();
		//std::cout << line << std::endl;
		lines.push_back(line);
	}
	//std::cout << std::endl;

	for(int i = 0; i < lines.size(); i++)
	{
		//std::cout << lines[i] << std::endl;
	}
	//std::cout << std::endl;

	int mult = 0;

	//parse numbers
	for(int y = 0; y < lines.size(); y++)
	{
		int num = 0, numLength = 0;
		Point start(-1,-1);
		bool inDig = false;
		for(int x = 0; x < length; x++)
		{
			//std::cout << (lines[y])[x];
			if (lines[y][x] >= 0x30 && lines[y][x] <= 0x39)
			{
				//we have a digit
				if(!inDig)
				{
					start.x = x;
					start.y = y;
				}
				mult = numLength > 0 ? 10 : 0;
				num = (num * mult) + (lines[y][x] - 0x30);
				if(x == (length - 1))
				{
					nums.push_back(Number(num, start, numLength));
					num = 0;
					numLength = 0;
					inDig = false;
					start = Point(-1, -1);
				}
				numLength++;
				inDig = true;
			}
			else if (inDig)
			{
				nums.push_back(Number(num, start, numLength));
				num = 0;
				numLength = 0;
				inDig = false;
				start = Point(-1, -1);
			}
			if(lines[y][x] == '*')
			{
				//std::cout << "X: " << x << ", Y: " << y << std::endl;
				points.push_back(Point(x,y));
			}
		}
		//std::cout << std::endl;
	}

	std::vector<Number>::iterator itr;

	for(itr = nums.begin(); itr < nums.end(); itr++)
	{
		//(*itr).disp();
		int y = (*itr).start.y;
		for(int x = (*itr).start.x; !((*itr).valid) && x < ((*itr).length + (*itr).start.x); x++)
		{
			//8 characters to check
			if(y != 0 && x != 0 && isSymbol(lines[y - 1][x - 1]))
			{
				(*itr).valid = true;
			}
			if(y != 0 &&isSymbol(lines[y - 1][x]))
			{
				(*itr).valid = true;
			}
			if(y != 0 && x != (length - 1) && isSymbol(lines[y - 1][x + 1]))
			{
				(*itr).valid = true;
			}
			if(x != 0 && isSymbol(lines[y][x - 1]))
			{
				(*itr).valid = true;
			}
			if(x != (length - 1) && isSymbol(lines[y][x + 1]))
			{
				(*itr).valid = true;
			}
			if(y != (lines.size() - 1) && x != 0 && isSymbol(lines[y + 1][x - 1]))
			{
				(*itr).valid = true;
			}
			if(y != (lines.size() - 1) && isSymbol(lines[y + 1][x]))
			{
				(*itr).valid = true;
			}
			if(y != (lines.size() - 1) && x != (length - 1) && isSymbol(lines[y + 1][x + 1]))
			{
				(*itr).valid = true;
			}

		}
	}

	int sum = 0;
	std::vector<Number>::iterator j;
	std::vector<Point>::iterator i;


	for(i = points.begin(); i < points.end(); i++)
	{
		std::vector<int> gears;
		for(j = nums.begin(); j < nums.end(); j++)
		{
			int y = (*j).start.y;
			bool done = false;
			for(int x = (*j).start.x; !done && x < ((*j).start.x + (*j).length); x++)
			{
				if((*i).isAdjacent(Point(x, y)))
				{
					done = true;
					//std::cout << "adjacent number" << std::endl;
					gears.push_back((*j).number);
				}
			}
		}
		if(gears.size() == 2)
		{
			sum += gears[0] * gears[1];
		}
	}

	std::cout << "Sum = " << sum << std::endl;

	return 0;
}

bool isSymbol(char chr)
{
	bool retVal = false;
	//potential gears.
	if(chr == '*')
	{
		//std::cout << chr << std::endl;
		retVal = true;
	}
	return retVal;
}
