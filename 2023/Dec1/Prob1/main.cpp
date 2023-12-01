#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

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
			//std::cout << "From Start: " << line[i] << std::endl;
			if(line[i] >= 0x30 && line[i] <= 0x39)
			{
				first = line[i] - 0x30;
				done = true;
			}
		}
		done = false;
		//loop from end to find last digit
		for(int j = (line.length() - 1); j > -1 && !done; j--)
		{
			//std::cout << "From End: " << line[j] << std::endl;
			if(line[j] >= 0x30 && line[j] <= 0x39)
			{
				last = line[j] - 0x30;
				done = true;
			}
		}
		//std::cout << "First = " << first << " Last = " << last << std::endl;
		partialSum = (10 * first) + last;
		sum += partialSum;
	}

	std::cout << "CumSum = " << sum << std::endl;
	return 0;
}
