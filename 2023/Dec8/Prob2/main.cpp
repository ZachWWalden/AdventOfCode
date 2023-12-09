#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <map>
#include <cmath>
#include <cstdint>

struct Node
{
	std::string key, left, right;
	bool done = false;

	void disp()
	{
		std::cout << "Location: " << key << ", L: " << left << ", R: " << right << ", DONE: " << done << std::endl;
	}
};

std::vector<std::string>* splitString(std::string string, char delimeter);
Node parseNode(std::string line);
std::string parseKey(std::string);

uint64_t lcmInt(uint64_t a, uint64_t b);
uint64_t gcdInt(uint64_t a, uint64_t b);

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

	std::map<std::string, Node> nodes;
	std::vector<std::string>::iterator lineItr = lines.begin();

	std::string moveSeq = *lineItr;
	lineItr++;
	lineItr++;
	Node node;

	for(lineItr; lineItr < lines.end(); lineItr++)
	{
		node = parseNode(*lineItr);
		nodes[node.key] = node;
		//node.disp();
	}
	std::map<std::string, Node>::iterator nodeItr;
	std::vector<Node> curNodes;

	for(nodeItr = nodes.begin(); nodeItr != nodes.end(); nodeItr++)
	{
		if((*nodeItr).first[2] == 'A')
		{
			curNodes.push_back(nodeItr->second);
			//nodeItr->second.disp();
		}
	}

	int numPaths = curNodes.size();
	uint64_t* pathLengths = new uint64_t[numPaths];

	for(int k = 0; k < numPaths; k++)
	{
		pathLengths[k] = 0;
	}

	bool done = false;
	u_int64_t cnt = 0;
	std::vector<Node>::iterator vodeItr;
	std::vector<Node> nextNodes;

	while(!done)
	{
		for(int i = 0; !done && i < moveSeq.length(); i++)
		{
			//loop through curNodes;
			int j = 0;
			for(vodeItr = curNodes.begin(); vodeItr < curNodes.end(); vodeItr++)
			{
				//nodeItr->second.disp();
				if (!(vodeItr->done))
				{
					if(moveSeq[i] == 'L')
					{
						nextNodes.push_back(nodes[vodeItr->left]);
					}
					else
					{
						nextNodes.push_back(nodes[vodeItr->right]);
					}
					pathLengths[j]++;
				}
				else
				{
					nextNodes.push_back(*vodeItr);
				}
				j++;
			}
			//erase curnodes, place all of next nodes into curNodes.
			curNodes.erase(curNodes.begin(), curNodes.end());
			int doneCnt = 0;
			for(vodeItr = nextNodes.begin(); vodeItr < nextNodes.end(); vodeItr++)
			{
				curNodes.push_back(*vodeItr);
				if(vodeItr->done)
					doneCnt++;
			}
			if(doneCnt == nextNodes.size())
				done = true;
			nextNodes.erase(nextNodes.begin(), nextNodes.end());
		}
	}

	uint64_t steps = 1;
	bool lcmPossible = true;

	for(int i = 0; i < numPaths; i++)
	{
		if(pathLengths[i] % moveSeq.length() != 0)
		{
			lcmPossible = false;
		}
		steps = lcmInt(steps, pathLengths[i]);
	}

	std::cout << lcmPossible << ", Steps = " <<  steps << std::endl;

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

Node parseNode(std::string line)
{
	std::size_t pos = line.find('=');
	line[pos] = ' ';

	Node retVal;

	std::vector<std::string>* lineElements = splitString(line, ' ');
	std::vector<std::string>::iterator itr = lineElements->begin();
	retVal.key = parseKey(*itr);
	itr++;
	retVal.left = parseKey(*itr);
	itr++;
	retVal.right = parseKey(*itr);

	if (retVal.key[2] == 'Z')
	{
		retVal.done = true;
	}

	delete lineElements;

	return retVal;
}

std::string parseKey(std::string key)
{
	std::string retVal = "";
	for(int i = 0; i < key.length(); i++)
	{
		if((key[i] >= 0x30 && key[i] <= 0x39) || (key[i] >= 0x41 && key[i] <= 0x5A))
			retVal += key[i];
	}

	return retVal;
}

uint64_t lcmInt(uint64_t a, uint64_t b)
{
	return (a*b)/gcdInt(a, b);
}
uint64_t gcdInt(uint64_t a, uint64_t b)
{
	return (b == 0) ? a : gcdInt(b, a % b);
}


