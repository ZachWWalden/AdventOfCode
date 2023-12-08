#include <cstddef>
#include <exception>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <map>

struct Node
{
	std::string key, left, right;

	void disp()
	{
		std::cout << "Location: " << key << ", L: " << left << ", R: " << right << std::endl;
	}
};

std::vector<std::string>* splitString(std::string string, char delimeter);
Node parseNode(std::string line);
std::string parseKey(std::string);

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

	Node curNode = nodes["AAA"];
	bool done = false;
	u_int64_t cnt = 0;

	while(!done)
	{
		for(int i = 0; !done && i < moveSeq.length(); i++)
		{
			if(moveSeq[i] == 'L')
				curNode = nodes[curNode.left];
			else
				curNode = nodes[curNode.right];

			if (curNode.key.compare("ZZZ") == 0)
			{
				done = true;
			}
			cnt++;
		}
	}

	std::cout << "Steps = " << cnt << std::endl;

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

	delete lineElements;

	return retVal;
}

std::string parseKey(std::string key)
{
	std::string retVal = "";
	for(int i = 0; i < key.length(); i++)
	{
		if(key[i] >= 0x41 && key[i] <= 0x5A)
			retVal += key[i];
	}

	return retVal;
}
