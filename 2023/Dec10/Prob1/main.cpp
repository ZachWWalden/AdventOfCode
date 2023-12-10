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
		}
	}

	int num = (loopLength & 1) + (loopLength >> 1);

	std::cout << "Farthest Point = " << num << std::endl;

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
