#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

enum CardType
{
	TWO = 0, THREE = 1, FOUR = 2, FIVE = 3, SIX = 4, SEVEN = 5, EIGHT = 6, NINE = 7, T = 8, J = 9, Q = 10, K = 11, A = 12
};
enum HandType
{
	HIGH_CARD = 0, PAIR = 1, TWO_PAIR = 2, THREE_KIND = 3, FULL_HOUSE = 4, FOUR_KIND = 5, FIVE_KIND = 6
};

CardType charToCard(char c);

struct Hand
{
	long int bet;
	const char* hand;
	HandType type;

	void disp()
	{
		std::cout << "Bet = " << bet << ", " << hand << std::endl;
	}
};

int parseInt(std::string parseVal);
std::vector<std::string>* splitString(std::string string, char delimeter);
Hand parseHand(std::vector<std::string>* lineSplit);
bool compareHand(const Hand& one, const Hand& two);

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
	std::vector<Hand> hands;

	while(std::getline(file, line))
	{
		hands.push_back(parseHand(splitString(line, ' ')));
	}

	std::sort(hands.begin(), hands.end(), compareHand);

	int winnings = 0;

	for(int i = 0; i < hands.size(); i++)
	{
		//hands[i].disp();
		winnings += hands[i].bet * (hands.size() - i);
	}

	std::cout << "Winnings = " << winnings << std::endl;

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

Hand parseHand(std::vector<std::string>* lineSplit)
{
	Hand hand;
	hand.bet = parseInt((*lineSplit)[1]);

	int cardCount[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	//13 card types
	for(int i = 0; i < (*lineSplit)[0].size(); i++)
	{
		cardCount[charToCard(((*lineSplit)[0])[i])]++;
	}

	int pairCount = 0, tripCount = 0, fourCount = 0, fiveCount = 0;
	for(int j = 0; j < 13; j++)
	{
		if(cardCount[j] == 2)
			pairCount++;
		else if(cardCount[j] == 3)
			tripCount++;
		else if (cardCount[j] == 4)
			fourCount++;
		else if (cardCount[j] == 5)
			fiveCount++;

	}
	if(fiveCount == 1)
		hand.type = HandType::FIVE_KIND;
	else if (fourCount == 1)
		hand.type = HandType::FOUR_KIND;
	else if (tripCount == 1)
	{
		if (pairCount == 1)
			hand.type = HandType::FULL_HOUSE;
		else
			hand.type = HandType::THREE_KIND;
	}
	else if (pairCount == 2)
		hand.type = HandType::TWO_PAIR;
	else if (pairCount == 1)
		hand.type = HandType::PAIR;
	else
		hand.type = HandType::HIGH_CARD;

	hand.hand = (*lineSplit)[0].c_str();

	return hand;
}

CardType charToCard(char c)
{
	switch (c)
	{
		case 'A' : return CardType::A;
		case 'K' : return CardType::K;
		case 'Q' : return CardType::Q;
		case 'J' : return CardType::J;
		case 'T' : return CardType::T;
		case '9' : return CardType::NINE;
		case '8' : return CardType::EIGHT;
		case '7' : return CardType::SEVEN;
		case '6' : return CardType::SIX;
		case '5' : return CardType::FIVE;
		case '4' : return CardType::FOUR;
		case '3' : return CardType::THREE;
		case '2' : return CardType::TWO;
	}
	return CardType::TWO;
}

bool compareHand(const Hand& one, const Hand& two)
{
	//return true if this Hand is greater or equal to passed hand
	bool retVal = false;
	if (one.type > two.type)
		retVal = true;
	else if (one.type == two.type)
	{
		//loop through char arrays.
		bool done = false;
		for(int i = 0; !done && i < 5; i++)
		{
			CardType O = charToCard(one.hand[i]), T = charToCard(two.hand[i]);
			if(O > T)
			{
				retVal = true;
				done = true;
			}
			else if (O == T)
			{
				retVal = false;
			}
			else
			{
				retVal = false;
				done = true;
			}
		}
	}
	else
	{
		retVal = false;
	}
	return retVal;
}
