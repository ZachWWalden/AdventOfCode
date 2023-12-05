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
#include <chrono>

struct Seed
{
	uint64_t startVal;
	uint64_t rangeLength;
	bool inRange(uint64_t seed)
	{
		return (seed >= startVal && seed < (startVal + rangeLength));
	}
};

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
	bool getSourceVal(uint64_t destVal, uint64_t* sourceVal)
	{
		bool valid = false;
		if (destVal >= destStart && destVal < (destStart + rangeLength))
		{
			valid = true;
			uint64_t offset = destVal - destStart;
			*sourceVal = sourceStart + offset;
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
std::vector<uint64_t>* mapOutputToSeed(uint64_t location);

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
	std::vector<Seed> seeds;

	while(std::getline(file, line))
	{
		lines.push_back(line);
	}

	//parse seeds;
	std::vector<std::string>::iterator lineItr = lines.begin();
	std::vector<std::string>* seedLine= splitString(*lineItr, ' ');
	std::vector<std::string>::iterator seedItr = seedLine->begin();
	seedItr++;
	for(; seedItr < seedLine->end(); seedItr+= 2)
	{
		Seed tmpSeed;
		tmpSeed.startVal = parseInt(*seedItr);
		tmpSeed.rangeLength = parseInt(*(seedItr+1));
		seeds.push_back(tmpSeed);
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
	std::vector<Seed>::iterator itr;//for looping through seeds.

	std::vector<uint64_t>* potSeeds;
	std::vector<uint64_t>::iterator potSeedItr;

	uint64_t minLocation = 0;
	bool minLocFound = false;

	while(!minLocFound)
	{
		//time this.
		//auto startTime = std::chrono::high_resolution_clock::now();
		potSeeds = mapOutputToSeed(minLocation);
		//auto endTime = std::chrono::high_resolution_clock::now();
		//auto execTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
		//std::cout << "ExecTime = " << (int)execTime.count() << std::endl;
		//loop through potential seeds, and compare each to all seeds.
		for(potSeedItr = potSeeds->begin(); potSeedItr < potSeeds->end(); potSeedItr++)
		{
			//loop through seeds to see if the seed is valid
			for(itr = seeds.begin(); !minLocFound && itr < seeds.end(); itr++)
			{
				if((*itr).inRange(*potSeedItr))
				{
					minLocFound = true;
				}
			}
		}
		delete potSeeds;
		minLocation++;
	}

	std::cout << "Min Location = " << minLocation - 1 << std::endl;

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

std::vector<uint64_t>* mapOutputToSeed(uint64_t location)
{
	std::vector<uint64_t>* seeds = new std::vector<uint64_t>;
	std::vector<uint64_t> imOne, imTwo;
	imOne.push_back(location);
	std::vector<std::vector<Map>*>::iterator i;
	std::vector<Map>::iterator j;
	bool buf = false;
	std::vector<uint64_t>::iterator k;

	for(i = maps.end() - 1; i >= maps.begin(); i--)
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
					if((*j).getSourceVal(*k, location))
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
					if((*j).getSourceVal(*k, location))
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
			seeds->push_back(*k);
		}
	}
	else
	{
		for(k = imOne.begin(); k < imOne.end(); k++)
		{
			seeds->push_back(*k);
		}
	}

	return seeds;
}

