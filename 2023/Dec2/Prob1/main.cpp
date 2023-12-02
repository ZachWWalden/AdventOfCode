#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#define RED 12
#define GREEN 13
#define BLUE 14

struct Set
{
	int red = 0, green = 0, blue = 0;
	Set(int nRed, int nGreen, int nBlue)
	{
		red = nRed;
		green = nGreen;
		blue = nBlue;
	}
	Set()
	{

	}
	void disp()
	{
		std::cout << "RED: " << red << " GREEN: " << green << " BLUE: " << blue << std::endl;
	}
};

struct Game
{
	int id;
	std::vector<Set>* sets;
	Game(std::vector<Set>* nSets, int nId)
	{
		id = nId;
		sets = nSets;
	}
	Game()
	{

	}
	void disp()
	{
		std::cout << "ID: " << id << std::endl;
		std::vector<Set>::iterator itr;
		for(itr = sets->begin(); itr < sets->end(); itr++)
		{
			(*itr).disp();
		}
	}
};

Game* parseLine(std::string line);
std::vector<Set>* parseSets(std::string sets);
Set parseSet(std::string set);
void parseColor(Set& retSet, std::string set, std::size_t pos);
bool validateGame(Game* game);

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		std::cout << "crap\n";
		return -1;
	}
	//*argv == filename
	std::string filename = argv[1];

	std::fstream file;
	file.open(filename.c_str(), std::ios::in);

	std::vector<Game*> games;
	std::string line;

	while (std::getline(file, line))
	{
		games.push_back(parseLine(line));
	}

	int sum = 0;
	std::vector<Game*>::iterator itr;

	for(itr = games.begin(); itr < games.end(); itr++)
	{
		sum += validateGame(*itr) ? (*itr)->id : 0;
		//free the memory
		delete (*itr)->sets;
		delete (*itr);
	}

	std::cout << "Sum = " << sum << std::endl;

	return 0;
}
//DONE
Game* parseLine(std::string line)
{
	std::cout << line << std::endl;
	std::size_t colPos = line.find(':');
	std::size_t spcPos = line.find(' ');

	Game* game = new Game();
	game->sets = parseSets(line.substr(colPos + 1));

	game->id = 0;
	for(int i = spcPos + 1; i < colPos; i++)
	{
		int multVal = (i > (spcPos + 1)) ? 10 : 0;
		game->id = game->id * (multVal) + (line[i] - 0x30);
	}
	//game->disp();
	return game;
}

std::vector<Set>* parseSets(std::string sets)
{
	std::vector<Set>* setVec = new std::vector<Set>;
	//std::cout << sets << std::endl << std::endl;
	size_t pos = sets.find(';');
	while (pos != std::string::npos)
	{
		setVec->push_back(parseSet(sets.substr(0, pos)));
		sets = sets.substr(pos + 1);
		//std::cout << sets << std::endl << std::endl;
		pos = sets.find(';');
	}
	setVec->push_back(parseSet(sets));
	return setVec;
}

Set parseSet(std::string set)
{
	//std::cout << set << std::endl;
	//no more than 2 commas
	Set retSet;
	std::size_t pos = set.find(',');
	while (pos != std::string::npos)
	{
		parseColor(retSet, set, pos);
		set = set.substr(pos + 1);
		//std::cout << set << std::endl;
		pos = set.find(',');
	}
	parseColor(retSet, set, pos);
	return retSet;
}

void parseColor(Set& retSet, std::string set, std::size_t pos)
{
	int numDigits = 0;
	int numColor = 0;
	bool done = false;
	for(int i = 0; i < pos && !done; i++)
	{
		//this is a single color
		//if digit
		if (set[i] >= 0x30 && set[i] <=0x39)
		{
			//we have a digit
			int multVal = (numDigits > 0) ? 10 : 0;
			numColor = numColor * (multVal) + (set[i] - 0x30);
			numDigits++;
		}
		//if r
		if (set[i] == 'r')
		{
			retSet.red	= numColor;
			done = true;
		}
		//if g
		if (set[i] == 'g')
		{
			retSet.green = numColor;
			done = true;
		}
		//if b
		if (set[i] == 'b')
		{
			retSet.blue	= numColor;
			done = true;
		}
	}
}

bool validateGame(Game* game)
{
	bool retVal = true;
	std::vector<Set>::iterator itr;
	for(itr = game->sets->begin(); itr < game->sets->end() && retVal; itr++)
	{
		//*itr = set
		if((*itr).red > RED || (*itr).green > GREEN || (*itr).blue > BLUE)
			retVal = false;
	}

	return retVal;
}
