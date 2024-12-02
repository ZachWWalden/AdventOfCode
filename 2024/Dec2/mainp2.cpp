#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

struct ReactorReport
{
	std::vector<std::vector<int64_t>*> report_plus_possibilities;
	uint32_t id;
	ReactorReport();
	ReactorReport(uint32_t n_id, std::vector<int64_t>* report)
	{
		id = n_id;
		report_plus_possibilities.push_back(report);
	}
	void addEditedReports(std::vector<int64_t>* report0, std::vector<int64_t>* report1, std::vector<int64_t>* report2)
	{
		report_plus_possibilities.pop_back();
		report_plus_possibilities.push_back(report0);
		report_plus_possibilities.push_back(report1);
		report_plus_possibilities.push_back(report2);
	}
	std::vector<std::vector<int64_t>*>* getReports()
	{
		return &report_plus_possibilities;
	}
};

std::vector<std::string>* splitString(std::string string, char delimeter);
std::vector<int64_t>* parseReactorReport(std::vector<std::string>* split_line);
void generateEditedReports(ReactorReport* report, int bad_idx);
int validateReport(std::vector<int64_t>* report);
void displayReport(std::vector<int64_t>* report);

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

	std::vector<ReactorReport> data;
	std::vector<std::string>* split_line;
	int id_cnt = 0;
	while(std::getline(file, line))
	{
		split_line = splitString(line, ' ');
		data.push_back(ReactorReport(id_cnt , parseReactorReport(split_line)));
		id_cnt++;
		//delete the current line
		delete split_line;
	}

	//loop through data
	for(int i = 0; i < data.size(); i++)
	{
		//loop through individual report for parsing
		std::vector<int64_t>* report = (*(data[i].getReports()))[0];
		int bad_idx = validateReport(report);
		if(bad_idx != -1)
		{
			generateEditedReports(&(data[i]), bad_idx);
		}
	}

	/*std::cout << std::endl << std::endl;*/

	int num = 0;

	for(int i = 0; i < data.size(); i++)
	{
		//loop through all reports;
		std::vector<std::vector<int64_t>*>* report_list = data[i].getReports();
		bool safe = false;
		for(int j = 0; j <  report_list->size() && !safe; j++)
		{
			int bad_idx = validateReport((*report_list)[j]);
			if(bad_idx == -1)
				safe = true;
		}
		if(safe)
		{
			num++;
			/*std::cout << "Safe";*/
		}
		/*std::cout << std::endl;*/
	}

	std::cout << "Total Safe = " << num << std::endl;

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

std::vector<int64_t>* parseReactorReport(std::vector<std::string>* split_line)
{
	std::vector<int64_t> *report = new std::vector<int64_t>;

	for(int i = 0; i < split_line->size(); i++)
	{
		report->push_back(std::stoi((*split_line)[i]));

	}
	return report;
}

void generateEditedReports(ReactorReport* report, int bad_idx)
{
	std::vector<std::vector<int64_t>*>* report_list = report->getReports();

	std::vector<int64_t> *rep_0 = new std::vector<int64_t>, *rep_1 = new std::vector<int64_t>, *rep_2 = new std::vector<int64_t>;

	for(int i = 0; i < (*report_list)[0]->size(); i++)
	{
		if(bad_idx != 0 && i == bad_idx - 1)
		{
			rep_1->push_back((*((*report_list)[0]))[i]);
			rep_2->push_back((*((*report_list)[0]))[i]);
		}
		else if(i == bad_idx)
		{
			rep_0->push_back((*((*report_list)[0]))[i]);
			rep_2->push_back((*((*report_list)[0]))[i]);
		}
		else if(i == (bad_idx + 1))
		{
			rep_0->push_back((*((*report_list)[0]))[i]);
			rep_1->push_back((*((*report_list)[0]))[i]);
		}
		else
		{
			rep_0->push_back((*((*report_list)[0]))[i]);
			rep_1->push_back((*((*report_list)[0]))[i]);
			rep_2->push_back((*((*report_list)[0]))[i]);
		}

	}

	/*std::cout << "Bad Index = " << bad_idx << std::endl;*/
	/*displayReport((*report_list)[0]);*/
	/*displayReport(rep_1);*/
	/*displayReport(rep_2);*/
	/*std::cout << std::endl;*/

	report->addEditedReports(rep_0, rep_1, rep_2);
	return;
}

int validateReport(std::vector<int64_t>* report)
{
	int bad_idx = -1;

	//loop through individual report for parsing
	bool safe = true, inc = false, dec = false;
	for(int j = 0; j < (*(report)).size() - 1 && safe; j++)
	{
		int64_t diff = (*(report))[j] - (*(report))[j + 1];
		/*std::cout << "diff = " << diff << ", report[j] = " << (*(report))[j] << ", report[j+1] = " << (*(report))[j + 1] << std::endl;*/
		int64_t abs_diff = std::abs(diff);
		if(abs_diff > 3 || abs_diff == 0)
		{
			safe = false;
			bad_idx = j;
			break;
		}
		//decreasing
		if(diff > 0)
		{
			dec = true;
			if(inc)
			{
				safe = false;
				bad_idx = j;
			}
		}
		//increasing
		else
		{
			inc = true;
			if(dec)
			{
				safe = false;
				bad_idx = j;
			}
		}
	}

	return bad_idx;
}

void displayReport(std::vector<int64_t>* report)
{
	for(auto i = report->begin(); i < report->end(); i++)
	{
		std::cout << *i << ", ";
	}
	std::cout << std::endl;
	return;
}
