#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

struct Race
{
	int time;
	int recDist;

	Race(int nTime, int nDist)
	{
		time = nTime;
		recDist = nDist;
	}

	void disp()
	{
		std::cout << "Time = " << time << ", Record Distance = " << recDist << std::endl;
	}
	/* D = R*T; R = hT; T = time - hT; D = -hT^2 + time*hT
	 * COMPLETE SQUARE: -D = hT^2 - time*hT; -D + (-time/2)^2 = hT^2 - time*hT + (-time/2)^2
	 * -D + (time^2/4) = (hT - (time/2))^2; +- sqrt(-D + (time^2/4)) + time/2 = hT
	 * time = 7, D = 9
	 * +- sqrt(3.25) + 3.5 = hT; hT = 1.697, 5.303
	 */
	int getDistance(int tHeld)
	{
		return time * tHeld - std::pow(tHeld, 2);
	}
	int getRecordRange()
	{
		int lower, upper;
		lower = std::ceil(((float) time)/2.0f - std::sqrt((std::pow((float)time, 2))/4.0f - (float)recDist));
		upper = std::floor(((float) time)/2.0f + std::sqrt((std::pow((float)time, 2))/4.0f - (float)recDist));

		lower = (recDist == getDistance(lower)) ? lower + 1 : lower;
		upper = (recDist == getDistance(upper)) ? upper - 1 : upper;
		//std::cout << "Lower = " << lower << ", Upper = " << upper << std::endl;
		return upper - lower + 1;
	}
};

int parseInt(std::string parseVal);
std::vector<std::string>* splitString(std::string string, char delimeter);

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
	std::vector<std::vector<std::string>*> lines;


	while(std::getline(file, line))
	{
		lines.push_back(splitString(line, ' '));
	}

	std::vector<std::string>::iterator a = lines[0]->begin(), b = lines[1]->begin();
	a++;
	b++;

	std::vector<Race> races;
	for(a, b; a < lines[0]->end() && b < lines[1]->end(); a++, b++)
	{
		races.push_back(Race(parseInt(*a), parseInt(*b)));
	}

	int product = 1;

	std::vector<Race>::iterator itr;
	for(itr = races.begin(); itr < races.end(); itr++)
	{
		//(*itr).disp();
		product *= (*itr).getRecordRange();
	}

	std::cout << "Product = " << product << std::endl;

	return 0;
}

int parseInt(std::string parseVal)
{
	int mult = 0, digCnt = 0;
	int num = 0;

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
