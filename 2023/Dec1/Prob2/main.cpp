#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

//If true is returned, digit has been edited.
bool isSpelledOut(int index, std::string line, int& digit);

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

	std::string line;
	int first, last;
	int sum = 0, partialSum = 0;

	while (std::getline(file, line))
	{
		bool done = false;
		//loop from beginning to find 1st digit
		for(int i = 0; i < line.length() && !done; i++)
		{
			if(line[i] >= 0x30 && line[i] <= 0x39)
			{
				first = line[i] - 0x30;
				done = true;
			}
			else if (line[i] == 'o' || line[i] == 't' || line[i] == 'f' || line[i] == 's' || line[i] == 'e' || line[i] == 'n')
			{
				if(isSpelledOut(i, line, first))
				{
					done = true;
				}
			}
		}
		done = false;
		//loop from end to find last digit
		for(int j = (line.length() - 1); j > -1 && !done; j--)
		{
			if(line[j] >= 0x30 && line[j] <= 0x39)
			{
				last = line[j] - 0x30;
				done = true;
			}
			else if (line[j] == 'o' || line[j] == 't' || line[j] == 'f' || line[j] == 's' || line[j] == 'e' || line[j] == 'n')
			{
				if(isSpelledOut(j, line, last))
				{
					done = true;
				}
			}
		}
		//std::cout << "First = " << first << " Last = " << last << std::endl << std::endl;
		partialSum = (10 * first) + last;
		sum += partialSum;
	}

	std::cout << "CumSum = " << sum << std::endl;
	return 0;
}

bool isSpelledOut(int index, std::string line, int& digit)
{
	if((index + 2) <= line.length() && line[index] == 'o' && line[index+1] == 'n' && line[index+2] == 'e')
	{
		digit = 1;
	}
	else if ((index + 2) <= line.length() && line[index] == 't' && line[index+1] == 'w' && line[index+2] == 'o')
	{
		digit = 2;
	}
	else if ((index + 4) <= line.length() && line[index] == 't' && line[index+1] == 'h' && line[index+2] == 'r' && line[index+3] == 'e' && line[index+4] == 'e')
	{
		digit = 3;
	}
	else if ((index + 3) <= line.length() && line[index] == 'f' && line[index+1] == 'o' && line[index+2] == 'u' && line[index+3] == 'r')
	{
		digit = 4;
	}
	else if ((index + 3) <= line.length() && line[index] == 'f' && line[index+1] == 'i' && line[index+2] == 'v' && line[index+3] == 'e')
	{
		digit = 5;
	}
	else if ((index + 2) <= line.length() && line[index] == 's' && line[index+1] == 'i' && line[index+2] == 'x')
	{
		digit = 6;
	}
	else if ((index + 4) <= line.length() && line[index] == 's' && line[index+1] == 'e' && line[index+2] == 'v' && line[index+3] == 'e' && line[index+4] == 'n')
	{
		digit = 7;
	}
	else if ((index + 4) <= line.length() && line[index] == 'e' && line[index+1] == 'i' && line[index+2] == 'g' && line[index+3] == 'h' && line[index+4] == 't')
	{
		digit = 8;
	}
	else if ((index + 3) <= line.length() && line[index] == 'n' && line[index+1] == 'i' && line[index+2] == 'n' && line[index+3] == 'e')
	{
		digit = 9;
	}
	//not a spelled out digit
	else
	{
		return false;
	}
	//Is a spelled out digit
	return true;
}
