#include <clocale>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <vector>
#include <unordered_map>

struct Lens
{
	std::string label = "";
	uint8_t focalLength;
	Lens()
	{
	}
	Lens(std::string nLabel, uint8_t nFocLen)
	{
		label = nLabel;
		focalLength = nFocLen;
	}
	bool isEqual(std::string lbl)
	{
		return !(label.compare(lbl));
	}
	void disp()
	{
		std::cout << label << ", FocLen: " << (int) focalLength << std::endl;
	}
};

std::vector<std::string>* splitString(std::string string, char delimeter);
Lens* hashAlgorithm(std::string string, uint64_t& boxNum, bool& operation);
uint64_t calcFocusPower(uint16_t boxNum, std::vector<Lens*>* lenses);

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
	std::vector<std::string>* lines;

	while(std::getline(file, line))
	{
		lines = splitString(line, ',');
	}
	uint64_t sum = 0, boxNum = 0;
	bool operation = false;
	Lens* lens;
	std::string label;
	std::unordered_map<uint8_t, std::vector<Lens*>*> boxes(256);
	for(int i = 0; i < 256; i++)
	{
		boxes[(uint8_t)i] = new std::vector<Lens*>;
	}


	for(std::string str : *lines)
	{
		std::vector<Lens*>* curBox;
		//std::cout << str << " ";
		lens = hashAlgorithm(str, boxNum, operation);
		//std::cout << " - " << tmp << std::endl;
		if(operation)
		{
			//add
			curBox = boxes[boxNum];
			bool alreadyThere = false;
			for (auto itr = curBox->begin(); !alreadyThere && itr < curBox->end(); itr++)
			{
				if ((*itr)->isEqual(lens->label))
				{
					(*itr)->focalLength = lens->focalLength;
					alreadyThere = true;
				}
			}
			if (!alreadyThere)
			{
				curBox->push_back(lens);
			}
		}
		else
		{
			//remove
			curBox = boxes[boxNum];
			//iterate or curBox;
			std::vector<Lens*>* newVec = new std::vector<Lens*>;
			for(std::vector<Lens*>::iterator curLens = curBox->begin(); curLens < curBox->end(); curLens++)
			{
				if(!((*curLens)->isEqual(lens->label)))
				{
					newVec->push_back(*curLens);
				}
			}
			delete curBox;
			boxes[boxNum] = newVec;
		}
	}

	for(int i = 0; i < 256; i++)
	{
		sum += calcFocusPower((uint16_t)i, boxes[i]);
	}

	std::cout << "Sum = " << sum << std::endl;

	return 0;
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

Lens* hashAlgorithm(std::string string, uint64_t& boxNum, bool& operation)
{
	Lens* lens = new Lens;
	uint64_t sum = 0;
	bool done = false;
	int focalIndex;
	for(int i = 0; !done && i < string.length(); i++)
	{
		if (string[i] == '=')
		{
			operation = true;
			focalIndex = i + 1;
			done = true;
		}
		else if (string[i] == '-')
		{
			operation = false;
			done = true;
		}
		else
		{
			sum += string[i];
			sum *= 17;
			sum = sum % 256;
			lens->label += string[i];
		}
	}
	if(operation)
	{
		//std::cout << string[focalIndex];
		lens->focalLength = string[focalIndex] - 0x30;
	}

	boxNum = sum;

	//lens->disp();

	return lens;
}

uint64_t calcFocusPower(uint16_t boxNum, std::vector<Lens*>* lenses)
{
	uint64_t focusPower = 0;
	for(int i = 0; i < lenses->size(); i++)
	{
		focusPower += (boxNum + 1) * (i + 1) * ((*lenses)[i])->focalLength;
		//(*i)->disp();
	}

	/*
	if(lenses->size() > 0)
	{
		std::cout << "Box Num: " << boxNum << std::endl;
		std::cout << "Focal Power " << (long) focusPower << std::endl << std::endl;
	}
	*/

	return focusPower;
}
