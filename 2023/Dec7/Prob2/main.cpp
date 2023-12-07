#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdint>

enum CardType
{
	J = 0, TWO = 1, THREE = 2, FOUR = 3, FIVE = 4, SIX = 5, SEVEN = 6, EIGHT = 7, NINE = 8, T = 9, Q = 10, K = 11, A = 12
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
	HandType typeJoker;

	void disp()
	{
		std::cout << "Bet = " << bet << ", " << hand;
		std::string str;
		switch (typeJoker)
		{
			case HandType::HIGH_CARD : str = "High Card"; break;
			case HandType::PAIR : str = "Pair"; break;
			case HandType::TWO_PAIR : str = "Two Pairs"; break;
			case HandType::THREE_KIND : str = "Three of a Kind"; break;
			case HandType::FULL_HOUSE : str = "Full House"; break;
			case HandType::FOUR_KIND : str = "Four of a Kind"; break;
			case HandType::FIVE_KIND : str = "Five of a Kind"; break;
		}
		std::cout << ", TYPE: " << str << std::endl;

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

	uint64_t winnings = 0;

	for(int i = 0; i < hands.size(); i++)
	{
		//hands[i].disp();
		winnings += (uint64_t)(hands[i].bet) * (uint64_t)(hands.size() - i);
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
	for(int j = 1; j < 13; j++)
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
	{
		hand.typeJoker = HandType::FIVE_KIND;
	}

	else if (fourCount == 1)
	{
		if (cardCount[0] == 1)
			hand.typeJoker = HandType::FIVE_KIND;
		else
			hand.typeJoker = HandType::FOUR_KIND;
	}

	//0, 1, or 2 jokers.
	else if (tripCount == 1)
	{
		if (cardCount[0] == 2)
		{
			hand.typeJoker = HandType::FIVE_KIND;
		}
		else if (cardCount[0] == 1)
		{
			hand.typeJoker = HandType::FOUR_KIND;
		}
		else
		{
			//no jokers
			if(pairCount == 1)
				hand.typeJoker = HandType::FULL_HOUSE;
			else
				hand.typeJoker = HandType::THREE_KIND;
		}
	}
	else if (pairCount == 2)
	{
		//we have 2 pairs that are not jokers. there can only be 1 joker to check for.
		if(cardCount[0] == 1)
			hand.typeJoker = HandType::FULL_HOUSE;
		else
			hand.typeJoker = HandType::TWO_PAIR;
	}

	else if (pairCount == 1)
	{
		//0, 1, 2 ,3 jokers
		if (cardCount[0] == 3)
		{
			hand.typeJoker = HandType::FIVE_KIND;
		}
		else if (cardCount[0] == 2)
		{
			hand.typeJoker = HandType::FOUR_KIND;
		}
		else if (cardCount[0] == 1)
		{
			hand.typeJoker = HandType::THREE_KIND;
		}
		else
		{
			hand.typeJoker = HandType::PAIR;
		}
	}
	else
	{
		//0, 1, 2 ,3, 4, 5 jokers
		if(cardCount[0] == 4 || cardCount[0] == 5)
		{
			hand.typeJoker = HandType::FIVE_KIND;
		}
		else if (cardCount[0] == 3)
		{
			hand.typeJoker = HandType::FOUR_KIND;
		}
		else if (cardCount[0] == 2)
		{
			hand.typeJoker = HandType::THREE_KIND;
		}
		else if (cardCount[0] == 1)
		{
			hand.typeJoker = HandType::PAIR;
		}
		else
		{
			hand.typeJoker = HandType::HIGH_CARD;
		}
	}

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
		case 'T' : return CardType::T;
		case '9' : return CardType::NINE;
		case '8' : return CardType::EIGHT;
		case '7' : return CardType::SEVEN;
		case '6' : return CardType::SIX;
		case '5' : return CardType::FIVE;
		case '4' : return CardType::FOUR;
		case '3' : return CardType::THREE;
		case '2' : return CardType::TWO;
		case 'J' : return CardType::J;
	}
	return CardType::TWO;
}

bool compareHand(const Hand& one, const Hand& two)
{
	//return true if this Hand is greater or equal to passed hand
	bool retVal = false;
	if (one.typeJoker > two.typeJoker)
		retVal = true;
	else if (one.typeJoker == two.typeJoker)
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
