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
				(*itr).valid = true;
			if(y != 0 &&isSymbol(lines[y - 1][x]))
				(*itr).valid = true;
			if(y != 0 && x != (length - 1) && isSymbol(lines[y - 1][x + 1]))
				(*itr).valid = true;
			if(x != 0 && isSymbol(lines[y][x - 1]))
				(*itr).valid = true;
			if(x != (length - 1) && isSymbol(lines[y][x + 1]))
				(*itr).valid = true;
			if(y != (lines.size() - 1) && x != 0 && isSymbol(lines[y + 1][x - 1]))
				(*itr).valid = true;
			if(y != (lines.size() - 1) && isSymbol(lines[y + 1][x]))
				(*itr).valid = true;
			if(y != (lines.size() - 1) && x != (length - 1) && isSymbol(lines[y + 1][x + 1]))
				(*itr).valid = true;

		}
	}

	int sum = 0;

	for(itr = nums.begin(); itr < nums.end(); itr++)
	{
		if ((*itr).valid)
		{
			sum += (*itr).number;
		}
	}

	std::cout << "Sum = " << sum << std::endl;

	return 0;
}

bool isSymbol(char chr)
{
	bool retVal = false;
	//@, #, $, %, &, *, -, =, +, /
	if(chr == '@' || chr == '#' || chr == '$' || chr == '%' || chr == '&' || chr == '*' || chr == '-' || chr == '=' || chr == '+' || chr == '/')
	{
		//std::cout << chr << std::endl;
		retVal = true;
	}
	return retVal;
}
