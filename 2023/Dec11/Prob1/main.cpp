#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <cstdint>
#include <map>
#include <cmath>

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
	vec2I difference(vec2I prevPos)
	{
		return vec2I(std::abs(x - prevPos.x), std::abs(y - prevPos.y));
	}
	void disp()
	{
		std::cout << "X: " << x << ", Y:" << y << std::endl;
	}
};

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

	while(std::getline(file, line))
	{
		lines.push_back(line);
	}

	std::vector<vec2I> galaxies;
	std::map<int, bool> cosmicX, cosmicY;

	//find galaxies., find cosmic expansion coords.
	for(auto i = lines.begin(); i < lines.end(); i++)
	{
		bool galFound = false;
		for(int j = 0; j < i->length(); j++)
		{
			if((*i)[j] == '#')
			{
				galFound = true;
				galaxies.push_back(vec2I(j, i - lines.begin()));
			}
		}
		if (!galFound)
		{
			cosmicY[i - lines.begin()] = true;
		}
		else {
			cosmicY[i - lines.begin()] = false;
		}
	}
	//
	for(int i = 0; i < lines[0].length(); i++)
	{
		bool galFound = false;
		for(auto j = lines.begin(); !galFound && j < lines.end();j++)
		{
			if((*j)[i] == '#')
			{
				galFound = true;
			}
		}
		if (galFound)
		{
			cosmicX[i] = false;
		}
		else
		{
			cosmicX[i] = true;
		}
	}
	/*
	for(vec2I gal : galaxies)
	{
		gal.disp();
	}
	*/

	int sum = 0;
	//make pairwise galaxies
	for(auto i = galaxies.begin(); i < galaxies.end() - 1; i++)
	{
		for(auto j = i + 1; j < galaxies.end(); j++)
		{
			vec2I riseRunAbs = (*i).difference(*j);
			int startY = (i->y >= j->y) ? j->y : i->y;
			int startX = (i->x >= j->x) ? j->x : i->x;
			int cosmicExpCnt = 0;
			//loop through x's
			for(int k = startX; k < riseRunAbs.x + startX; k++)
			{
				//std::cout << "K : " << k << std::endl;
				if(cosmicX[k])
				{
					cosmicExpCnt++;
				}
			}
			//loop through y's.
			for(int l = startY; l < riseRunAbs.y + startY; l++)
			{
				//std::cout << "L : " << l << std::endl;
				if(cosmicY[l])
				{
					cosmicExpCnt++;
				}
			}
			/*
			std::cout << "RiseRun: ";
			riseRunAbs.disp();
			std::cout << "(" << (int) (1 + i - galaxies.begin()) << ", " << (int) (1 + j - galaxies.begin()) << ")" << std::endl;
			std::cout << "Cosmic Exp Cnt = " << cosmicExpCnt << std::endl;
			std::cout << "Pair Sum = " << cosmicExpCnt + riseRunAbs.x + riseRunAbs.y << std::endl << std::endl;
			*/
			sum += cosmicExpCnt + riseRunAbs.x + riseRunAbs.y;
		}
	}


	std::cout << "Sum = " << sum << std::endl;

	return 0;
}

//example 4/3 slope becomes 5/4
//shortest path = rise+run + N expansion crossings.
