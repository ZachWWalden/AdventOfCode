#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Rule
{
	int x,y;
	Rule()
	{

	}
	Rule(int n_x, int n_y)
	{
		x = n_x;
		y = n_y;
	}
	void disp()
	{
		std::cout << x << " before " << y << std::endl;
	}
	bool applies(std::vector<int> *update)
	{
		bool x_fnd = false, y_fnd = false;
		for(int i = 0; i < update->size() && (!x_fnd || !y_fnd); i++)
		{
			if((*update)[i] == x)
				x_fnd = true;
			if((*update)[i] == y)
				y_fnd = true;
		}
		return (x_fnd && y_fnd) ? true : false;
	}
};

struct FreqCount
{
	int number, count;
};

std::vector<std::string>* splitString(std::string string, char delimeter);
void dispUpdate(std::vector<int> *update);
void dispRuleSet(std::vector<Rule> *update);

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

	std::vector<Rule> rules;
	std::vector<std::vector<int>*> updates;
	std::vector<std::string>* split_string;
	bool parsing_rules = true;

	while(std::getline(file, line))
	{
		if(line.length() == 0)
			parsing_rules = false;
		if(parsing_rules)
		{
			int nums[2];
			split_string = splitString(line, '|');
			for(int i = 0; i < split_string->size(); i++)
			{
				nums[i] = std::stoi((*split_string)[i]);
			}
			rules.push_back(Rule(nums[0], nums[1]));
		}
		else
		{
			if(line.length() != 0)
			{
				std::vector<int> *update = new std::vector<int>;
				split_string = splitString(line, ',');
				for(int i = 0; i < split_string->size(); i++)
				{
					update->push_back(std::stoi((*split_string)[i]));
				}
				updates.push_back(update);
			}
		}
	}

	int num = 0;

	for(int i = 0; i< updates.size(); i++)
	{
		std::vector<Rule> *applicable_rules = new std::vector<Rule>;
		for(int j = 0; j < rules.size(); j++)
		{
			if(rules[j].applies(updates[i]))
			{
				applicable_rules->push_back(rules[j]);
			}
		}
		bool rules_met = true;
		for(int k = 0; k < applicable_rules->size() && rules_met; k++)
		{
			/*(*applicable_rules)[k].disp();*/
			int x_idx = 0, y_idx = 0;
			for(int l = 0; l < updates[i]->size(); l++)
			{
				if(((*updates[i]))[l] == (*applicable_rules)[k].x)
					x_idx = l;
				if(((*updates[i]))[l] == (*applicable_rules)[k].y)
					y_idx = l;
			}
			/*std::cout << "X Index = " << x_idx << ", Y Index = " << y_idx << std::endl;*/
			if(y_idx < x_idx)
				rules_met = false;
		}
		if(!rules_met)
		{
			//fix that shit
			//n^2 algo because my comp is fast enough lmao.
			//compute Y column numeral frequency;
			//loop through update
			bool done = false;
			for(int m = 0; m < (*(updates[i])).size() && !done; m++)
			{
				int cnt = 0;
				for(int n = 0; n < applicable_rules->size(); n++)
				{
					if((*(updates[i]))[m] == (*applicable_rules)[n].y)
						cnt++;
				}
				if(cnt == updates[i]->size() >> 1)
				{
					//middle num found.
					done = true;
					num += (*(updates[i]))[m];
				}
			}
		}
		delete applicable_rules;
	}

	std::cout << "Middle Page Sum = " << num << std::endl;

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

void dispUpdate(std::vector<int> *update)
{
	for(int i = 0; i < update->size(); i++)
	{
		std::cout << (*update)[i] << ", ";
	}
	std::cout << std::endl;
}
void dispRuleSet(std::vector<Rule> *update)
{
	for(int i = 0; i < update->size(); i++)
	{
		(*update)[i].disp();
	}
	std::cout << std::endl;
}
