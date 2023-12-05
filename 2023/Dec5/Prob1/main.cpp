#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <cstdint>
#include <cmath>

struct Map
{
	uint64_t destStart;
	uint64_t sourceStart;
	uint64_t rangeLength;
	void disp()
	{
		std::cout << "destStart: " << destStart << ", sourceStart: " << sourceStart << ", Length: " << rangeLength << std::endl;
	}
	bool getDestVal(uint64_t sourceVal, uint64_t* destVal)
	{
		bool valid = false;
		if (sourceVal >= sourceStart && sourceVal < (sourceStart + rangeLength))
		{
			valid = true;
			uint64_t offset = sourceVal - sourceStart;
			*destVal = destStart + offset;
			//std::cout << "DESTVAL = " << *destVal << std::endl;
			//std::cout << "SourceVal = " << sourceVal << ", " << std::endl;
			//disp();
			//std::cout << std::endl;
		}
		return valid;
	}
};

uint64_t parseInt(std::string parseVal);
std::vector<std::string>* splitString(std::string string, char delimeter);
Map parseMap(std::vector<std::string>* line);
std::vector<uint64_t>* mapSeedToOutput(uint64_t seed);

std::vector<std::vector<Map>*> maps;

std::string mapNames[7] = {"seed-to-soil", "soil-to-fertilizer", "fertilizer-to-water", "water-to-light", "light-to-temperature", "temperature-to-humidity"};

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
	std::vector<uint64_t> seeds;

	while(std::getline(file, line))
	{
		lines.push_back(line);
	}

	//parse seeds;
	std::vector<std::string>::iterator lineItr = lines.begin();
	std::vector<std::string>* seedLine= splitString(*lineItr, ' ');
	std::vector<std::string>::iterator seedItr = seedLine->begin();
	seedItr++;
	for(; seedItr < seedLine->end(); seedItr++)
	{
		seeds.push_back(parseInt(*seedItr));
	}
	lineItr++;

	for(; lineItr < lines.end(); lineItr++)
	{
		//if last char != :
		if ((*lineItr).find(':') == std::string::npos && (*lineItr).compare("") != 0)
		{
			std::vector<Map>* mapSet = new std::vector<Map>;
			while((*lineItr).compare("") != 0)
			{
				mapSet->push_back(parseMap(splitString(*lineItr, ' ')));
				lineItr++;
			}
			maps.push_back(mapSet);
			//std::cout << std::endl;
		}
	}

	//loop through lines. separating by set.
	std::vector<std::vector<Map>*>::iterator i;
	std::vector<Map>::iterator j;
	std::vector<uint64_t>::iterator seed;

	std::vector<uint64_t>* locations;
	uint64_t minLocation = 0xFFFFFFFFFFFFFFFF;

	for(seed = seeds.begin(); seed < seeds.end(); seed++)
	{
		//std::cout << "SEED = " << *seed << std::endl << std::endl << std::endl;
		locations = mapSeedToOutput(*seed);
		std::vector<uint64_t>::iterator locItr;
		for(locItr = locations->begin(); locItr < locations->end(); locItr++)
		{
			//std::cout << "LOCATION = " << *locItr << std::endl;
			if((*locItr) < minLocation)
			{
				minLocation = *locItr;
			}
		}
	}

	std::cout << "Min Location = " << minLocation << std::endl;

	return 0;
}

uint64_t parseInt(std::string parseVal)
{
	uint64_t mult = 0, digCnt = 0;
	uint64_t num = 0;

	for(int i = 0; i < parseVal.length(); i++)
	{
		if(parseVal[i] >= 0x30 && parseVal[i] <= 0x39)
		{
			mult = (digCnt > 0) ? 10 : 0;
			num = (num * mult) + (parseVal[i] - 0x30);
			digCnt++;
			//std::cout << "CHAR: " << parseVal[1] << ", NUM: " << num << std::endl;
		}
	}

	//std::cout << parseVal << ", LEN: " << parseVal.length() << ", VAL: " << num << std::endl;

	return num;
}

std::vector<std::string>* splitString(std::string string, char delimeter)
{
	std::vector<std::string>* split = new std::vector<std::string>;

	std::size_t pos = 0;
	std::string splitVal;

	while (pos != std::string::npos)
	{
		//std::cout << string << std::endl;
		pos = string.find(delimeter);
		//2 sub str, the
		splitVal = string.substr(0, pos);
		if(splitVal.length() > 0)
			split->push_back(splitVal); //push_back 1st token separated by "delimeter"
		string = string.substr(pos + 1);
	}

	/*
	std::vector<std::string>::iterator j;
	for(j = split->begin(); j < split->end();j++)
	{
		std::cout << (*j) << std::endl;
	}
	*/
	return split;
}

Map parseMap(std::vector<std::string>* line)
{
	std::vector<std::string>::iterator j = line->begin();
	Map map;
	map.destStart = parseInt(*j);
	j++;
	map.sourceStart = parseInt(*j);
	j++;
	map.rangeLength = parseInt(*j);
	//map.disp();

	return map;
}
//I'm forgetting direct maps. TODO if no map is found. push the id.
std::vector<uint64_t>* mapSeedToOutput(uint64_t seed)
{
	std::vector<uint64_t>* locations = new std::vector<uint64_t>;
	std::vector<uint64_t> imOne, imTwo;
	imOne.push_back(seed);
	std::vector<std::vector<Map>*>::iterator i;
	std::vector<Map>::iterator j;
	bool buf = false;
	std::vector<uint64_t>::iterator k;

	for(i = maps.begin(); i < maps.end(); i++)
	{
		//std::cout << mapNames[i - maps.begin()] << std::endl;
		//std::cout << "Seed -> soil size = " << (*i)->size() << std::endl;
		for(j = (*i)->begin(); j < (*i)->end(); j++)
		{
			//(*j).disp();
			//std::cout << j - (*i)->begin() << std::endl;
			uint64_t* location = new uint64_t;
			*location = 0xFFFFFFFFFFFFFFFF;
			if (buf)
			{
				//std::cout << "LOC in imTwo\n";
				//use im one.
				if(j == (*i)->begin())
					imOne.clear();
				//loop through values in imTwo
				for(k = imTwo.begin(); k < imTwo.end(); k++)
				{
					if((*j).getDestVal(*k, location))
					{
						imOne.push_back(*location);
					}
					else
						delete location;
				}
			}
			else
			{
				//std::cout << "LOC in imOne\n";
				//use im two
				if(j == (*i)->begin())
					imTwo.clear();
				for(k = imOne.begin(); k < imOne.end(); k++)
				{
					if((*j).getDestVal(*k, location))
					{
						imTwo.push_back(*location);
					}
					else
						delete location;
				}
			}
			//std::cout << "mapLocation = " << location << std::endl;
		}
		if(buf)
		{
			buf = false;
			if(imOne.size() == 0)
			{
				//push back all values in imTwo
				for(k = imTwo.begin(); k < imTwo.end(); k++)
				{
					imOne.push_back(*k);
				}
			}
		}
		else
		{
			buf = true;
			if(imTwo.size() == 0)
			{
				//push back all values in imTwo
				for(k = imOne.begin(); k < imOne.end(); k++)
				{
					imTwo.push_back(*k);
				}
			}
		}
	}

	if (buf)
	{
		//loop through values in imTwo
		for(k = imTwo.begin(); k < imTwo.end(); k++)
		{
			locations->push_back(*k);
		}
	}
	else
	{
		for(k = imOne.begin(); k < imOne.end(); k++)
		{
			locations->push_back(*k);
		}
	}

	return locations;
}
