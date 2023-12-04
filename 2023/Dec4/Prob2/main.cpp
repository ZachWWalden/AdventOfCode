#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

struct Card
{
	int id, matchCnt;
};

int parseInt(std::string parseVal);
std::vector<std::string>* splitString(std::string string, char delimeter);
Card parseCard(std::vector<std::string>* card);
int parseMatch(Card card);


std::vector<Card> cards;

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

	std::vector<std::vector<std::string>*>::iterator i;


	for(i = lines.begin(); i < lines.end(); i++)
	{
		cards.push_back(parseCard(*i));
	}

	std::cout << std::endl;

	int sum = 0;
	std::vector<Card>::iterator k;

	for(k = cards.begin(); k < cards.end(); k++)
	{
		//std::cout << "GUNUINE: ID: " << (*k).id << ", CNT: " << (*k).matchCnt << std::endl;
		sum += parseMatch(*k);
	}

	std::cout << "Sum = " << sum << std::endl;

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

Card parseCard(std::vector<std::string>* card)
{
	int matchCnt = 0;
	std::vector<int> winningNums;
	Card retCard;
	bool done = false;
	//ignore 1st two values in the passed vector
	std::vector<std::string>::iterator j = card->begin();
	j++;
	retCard.id = parseInt(*j);
	j++;

	while((*j) != "|")
	{
		//std::cout << *j << std::endl;
		winningNums.push_back(parseInt(*j));
		j++;
	}
	j++;
	while (j < card->end())
	{
		std::vector<int>::iterator k;
		for(k = winningNums.begin(); k < winningNums.end(); k++)
		{
			if ((*k) == parseInt(*j))
			{
				//std::cout << "k = " << *k << " j = " << *j << std::endl;
				matchCnt++;
			}
		}
		j++;
	}

	retCard.matchCnt = matchCnt;
	//std::cout << "ID: " << retCard.id << ", CNT: " << retCard.matchCnt << std::endl;
	return retCard;
}

int parseMatch(Card card)
{
	int numScratchCards = 1;
	if (card.matchCnt > 0)
	{
		for(int i = card.id; i < (card.id + card.matchCnt); i++)
		{
			//std::cout << "COPY: ID: " << cards[i].id << ", CNT: " << cards[i].matchCnt << std::endl;
			numScratchCards += parseMatch(cards[i]);
		}
	}
	return numScratchCards;
}
