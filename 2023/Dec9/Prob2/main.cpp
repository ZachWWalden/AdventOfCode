#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int parseInt(std::string parseVal);
std::vector<std::string>* splitString(std::string string, char delimeter);
std::vector<int>* parseLine(std::vector<std::string>* line);
int solveLastNum(std::vector<int>* seq);

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
	//	 lines        line       ints      k j i
	std::vector<std::vector<int>*> lines;
	//lines[]

	int	sum = 0;

	while(std::getline(file, line))
	{
		sum += solveLastNum(parseLine(splitString(line, ' ')));
		//std::cout << std::endl;
	}

	std::cout << "Sum = " << sum << std::endl;

	return 0;
}

int parseInt(std::string parseVal)
{
	int mult = 0, digCnt = 0;
	int num = 0;
	int negative = 1;

	for(int i = 0; i < parseVal.length(); i++)
	{
		if(parseVal[i] >= 0x30 && parseVal[i] <= 0x39)
		{
			mult = (digCnt > 0) ? 10 : 0;
			num = (num * mult) + (parseVal[i] - 0x30);
			digCnt++;
			//std::cout << "CHAR: " << parseVal[1] << ", NUM: " << num << std::endl;
		}
		else if (parseVal[i] == '-')
		{
			negative = -1;
		}
	}

	//std::cout << parseVal << ", LEN: " << parseVal.length() << ", VAL: " << num << std::endl;

	return negative * num;
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


std::vector<int>* parseLine(std::vector<std::string>* lineSplit)
{
	std::vector<int>* ints = new std::vector<int>;

	for (std::string str : *(lineSplit))
	{
		ints->push_back(parseInt(str));
	}

	delete lineSplit;

	return ints;
}

int solveLastNum(std::vector<int>* seq)
{
	std::vector<int>* nxtSeq = new std::vector<int>;
	int diff = 0;
	/*
	for(int j : *seq)
	{
		std::cout << j << " ";
	}
	std::cout << std::endl;
	*/

	for(int i = 0; i < seq->size() - 1; i++)
	{
		nxtSeq->push_back(seq->at(i + 1) - seq->at(i));
	}
	int doneCnt = 0;
	for (int num : *nxtSeq)
	{
		if(num == 0)
			doneCnt++;
	}
	if(!(doneCnt == nxtSeq->size()))
		diff = solveLastNum(nxtSeq);

	delete nxtSeq;
	return *(seq->begin()) - diff;

}
