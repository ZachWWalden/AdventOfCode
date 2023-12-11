#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

struct vec2I
{
	int x, y;
	vec2I()
	{

	}
	vec2I(int nX, int nY)
	{
		x = nX;
		y = nY;
	}
	bool isEqual(vec2I vec)
	{
		return x == vec.x && y == vec.y;
	}
	vec2I difference(vec2I prevPos)
	{
		return vec2I(x - prevPos.x, y - prevPos.y);
	}
	void disp()
	{
		std::cout << "X: " << x << ", Y:" << y << std::endl;
	}
	void add(vec2I point)
	{
		x += point.x;
		y += point.y;
	}
};
//LAST MOVE = ENTRY POINT
	// |: (x, y -1), (x, y + 1): (0,-1), (0,1)
	// -: (x-1, y), (x+1, y): (-1,0), (1,0)
	// L: (x, y+1), (x-1, y): (0,1), (-1,0)
	// J: (x, y+1), (X+1, y): (0,1), (1, 0)
	// 7: (x, y-1), (x+1, y): (0,-1), (1,0)
	// F: (x, y-1), (x-1, y): (0,-1), (-1,0)
vec2I validateEntry[6][2] = {{vec2I(0, -1), vec2I(0, 1)},
							 {vec2I(-1,0), vec2I(1,0)},
							 {vec2I(0,1), vec2I(-1,0)},
							 {vec2I(0,1), vec2I(1,0)},
							 {vec2I(0,-1), vec2I(1,0)},
							 {vec2I(0,-1), vec2I(-1,0)}};
//EXIT = -1 * ENTY
	// |: (x, y - 1), (x, y + 1): (0,-1), (0,1)
	// -: (x-1, y), (x+1, y): (-1,0), (1,0)
	// L: (x+1, y), (x, y-1): (1,0), (0,-1)
	// J: (x-1, y), (x, y-1): (-1,0), (0, -1)
	// 7: (x-1, y), (x, y+1): (-1,0), (0,1)
	// F: (x+1, y), (x, y+1): (1,0), (0,1)
vec2I validateExit[6][2] = {{vec2I(0, -1), vec2I(0, 1)},
							{vec2I(-1,0), vec2I(1,0)},
							{vec2I(1,0), vec2I(0,-1)},
							{vec2I(-1,0), vec2I(0,-1)},
							{vec2I(-1,0), vec2I(0,1)},
							{vec2I(1,0), vec2I(0,1)}};

vec2I sExits[4] = {vec2I(-1, 0),
				   vec2I(1, 0),
				   vec2I(0, -1),
				   vec2I(0, 1)};

std::vector<std::string> lines;

int charToInt(char chr);
vec2I moveToNextChar(vec2I prevPos, vec2I pos, char chr);
bool isEnc(bool** isLoop, vec2I point);
bool checkDir(bool** isLoop, vec2I point, vec2I delta);

template <typename T>
T** allocDoublePointer(int y, int x, T initial_value)
{
	T** ret_value = new T*[y];
	for(int i = 0; i < y; i++)
	{
		ret_value[i] = new T[x];
		for(int j = 0; j < x; j++)
		{
			ret_value[i][j] = initial_value;
		}
	}
	return ret_value;
}

//De-Allocate Triple Pointer
template <typename T>
void deallocDoublePointer(T** pointer, int y)
{
	for(int i = 0; i < y; i++)
	{
		delete [] pointer[i];
	}
	delete [] pointer;
}

void dispDoublePointer(bool** pointer, int y, int x);

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

	while(std::getline(file, line))
	{
		lines.push_back(line);
	}

	std::vector<vec2I> loopCoords;

	vec2I sPos(0,0);
	bool done = false;
	for(sPos.y = 0; !done && sPos.y < lines.size(); sPos.y++)
	{
		for(sPos.x = 0; !done && sPos.x < lines[sPos.y].length(); sPos.x++)
		{
			if (lines[sPos.y][sPos.x] == 'S')
			{
				done = true;
			}
		}
	}

	sPos.x--; sPos.y--;
	loopCoords.push_back(sPos);
	//sPos.disp();


	//My S has to be an L;
	char curChar = ' ';
	int loopLength = 1;
	vec2I pos = sPos, prevPos = sPos, tmp;

	//handle the first value
	//Either exit move is valid.
	//loop through S exit moves
	vec2I exitMove;
	bool valid = false;
	for(int i = 0; !valid && i < 4; i++)
	{
		exitMove = sExits[i];
		if(!((pos.x + exitMove.x) < 0 || (pos.x + exitMove.x) == lines[0].size() || (pos.y + exitMove.y) < 0 || (pos.y + exitMove.y) == lines.size()))
		{
			int outerIdx = charToInt(lines[pos.y + exitMove.y][pos.x + exitMove.x]);
			for(int j = 0; !valid && j < 2; j++)
			{
				if (exitMove.isEqual(validateEntry[outerIdx][j]))
				{
					//we have a valid entry condition
					valid = true;
				}
			}
		}
	}
	/*
	std::cout << "CUR POS: ";
	pos.disp();
	std::cout << "EXIT MOVE: ";
	exitMove.disp();
	std::cout << std::endl;
*/
	pos.x = pos.x + exitMove.x;
	pos.y = pos.y + exitMove.y;
	curChar = lines[pos.y][pos.x];

	done = false;
	while (!done)
	{
		/*
		std::cout << std::endl << "Cur Pos";
		pos.disp();
		std::cout << "CHAR: " << curChar << std::endl;
		*/

		tmp = pos;
		pos = moveToNextChar(prevPos, pos, curChar);
		//std::cout << std::endl;
		prevPos = tmp;
		if((pos.isEqual(sPos)) || (pos.isEqual(vec2I(-1,-1))))
		{
			//we are done
			done = true;
		}
		else
		{
			curChar = lines[pos.y][pos.x];
			loopLength++;
			loopCoords.push_back(pos);
		}
	}

	bool** isLoop = allocDoublePointer<bool>(lines.size(), lines[0].length(), false);
	bool** isEnclosed = allocDoublePointer<bool>(lines.size(), lines[0].length(), true);
	bool** counted = allocDoublePointer<bool>(lines.size(), lines[0].length(), false);

	vec2I max(0, 0), min(lines[0].length(), lines.size());
	for(vec2I point : loopCoords)
	{
		isLoop[point.y][point.x] = true;
		//if point is part of loop, it cannot be enclosed,
		isEnclosed[point.y][point.x] = false;
		if (point.x < min.x)
		{
			min.x = point.x;
		}
		if (point.y < min.y)
		{
			min.y = point.y;
		}
		if(point.x > max.x)
		{
			max.x = point.x;
		}
		if(point.y > max.y)
		{
			max.y = point.y;
		}
	}

	//loop over lines
	for(auto line : lines)
	{

	}

	if (min.x == 0)
	{
		min.x++;
	}
	if (min.y == 0)
	{
		min.y++;
	}
	if(max.x == (lines[0].length() - 1))
	{
		max.x--;
	}
	if(max.y == (lines.size() - 1))
	{
		max.y--;
	}

	std::vector<vec2I> potPoints;
	for(int y = 0; y < lines.size(); y++)
	{
		for(int x = 0; x < lines[y].length(); x++)
		{
			if(!(x >= min.x && x <= max.x && y >= min.y && y <= max.y))
			{
				isEnclosed[y][x] = false;
			}
			else if (!(isLoop[y][x]))
			{
				potPoints.push_back(vec2I(x,y));
			}
		}
	}

	dispDoublePointer(isLoop, lines.size(), lines[0].length());
	dispDoublePointer(isEnclosed, lines.size(), lines[0].length());

	int sum = 0;
	for(vec2I point : potPoints)
	{
		if(isEnc(isLoop, point))
		{

			//point.disp();
			counted[point.y][point.x] = true;
			sum++;
			isLoop[point.y][point.x] = true;
		}
	}
	//int num = (loopLength & 1) + (loopLength >> 1);

	dispDoublePointer(counted, lines.size(), lines[0].length());

	std::cout << "Num Enclosed = " << sum << std::endl;

	file.close();
	deallocDoublePointer(isLoop, lines.size());

	return 0;
}

int charToInt(char chr)
{
	switch (chr)
	{
		case '|' : return 0;
		case '-' : return 1;
		case 'L' : return 2;
		case 'J' : return 3;
		case '7' : return 4;
		case 'F' : return 5;
	}
	return -1;
}

vec2I moveToNextChar(vec2I prevPos, vec2I pos, char chr)
{
	int outerIdx = charToInt(chr);
	vec2I lastMove = pos.difference(prevPos);
	/*
	std::cout << "LAST MOVE: ";
	lastMove.disp();
	*/

	int innerIdx = -1;
	bool done = false;
	//loop through entry statements
	for(int i = 0; !done && i < 2; i++)
	{
		if (lastMove.isEqual(validateEntry[outerIdx][i]))
		{
			done = true;
			innerIdx = i;
		}
	}
	/*
	if (done)
	{
		std::cout << "EXIT MOVE: ";
		validateExit[outerIdx][innerIdx].disp();
	}
	*/
	vec2I exitMove = validateExit[outerIdx][innerIdx];

	return (!done) ? vec2I(-1,-1) : vec2I(pos.x + exitMove.x, pos.y + exitMove.y);
}

void dispDoublePointer(bool** pointer, int y, int x)
{
	for(int rows = 0; rows < y; rows++)
	{
		for(int cols = 0; cols < x; cols++)
		{
			std::cout << pointer[rows][cols];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
bool isEnc(bool** isLoop, vec2I point)
{
	return checkDir(isLoop, point, vec2I(-1,-1)) && checkDir(isLoop, point, vec2I(0,-1)) && checkDir(isLoop, point, vec2I(1,-1)) && checkDir(isLoop, point, vec2I(-1,0)) && checkDir(isLoop, point, vec2I(1,0)) && checkDir(isLoop, point, vec2I(-1,1)) && checkDir(isLoop, point, vec2I(0,1)) && checkDir(isLoop, point, vec2I(1,1));
}

bool checkDir(bool** isLoop, vec2I point, vec2I delta)
{

	point.add(delta);
	bool retVal = false;
	bool done = false;
	while (!done)
	{
		//check isLoop
		if(isLoop[point.y][point.x])
		{
			retVal = true;
			done = true;
		}
		else if (point.x == 0 || point.y == 0 || point.x == (lines[0].length() - 1) || point.y == (lines.size() - 1))
		{
			done = true;
		}
		point.add(delta);
	}

	return retVal;
}
