#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>

struct Point3d
{
	int64_t x, y, z;
	void *circuit = nullptr; //pointer to the circuit this point is contained in.
	Point3d(int64_t n_x, int64_t n_y, int64_t n_z)
	{
		x = n_x;
		y = n_y;
		z = n_z;
	}
	Point3d()
	{

	}
	void disp()
	{
		std::cout << "X = " << x << ", Y = " << y << ", Z = " << z << std::endl;
	}
};

struct Distance
{
	Point3d *pt_a, *pt_b;
	uint64_t distance;
	Distance(Point3d *n_pt_a, Point3d *n_pt_b, uint64_t n_distance)
	{
		pt_a = n_pt_a;
		pt_b = n_pt_b;
		distance = n_distance;
	}
	void disp()
	{
		std::cout << "Point A" << std::endl;
		pt_a->disp();
		std::cout << "Point B" << std::endl;
		pt_b->disp();
		std::cout << "Distance = " << distance << std::endl << std::endl;
	}
};

struct Circuit
{
	std::vector<Point3d*> points;
	bool deleted = false;
};

std::vector<std::string>* splitString(std::string string, char delimeter);
Point3d parsePoint3d(std::vector<std::string> *pt_str);
uint64_t calcPoint3dDistance(Point3d a, Point3d b);
bool isPointEqual(Point3d *a, Point3d *b);
bool compareDistance(const Distance &a, const Distance &b);

void combineCircuit(Circuit *dest, Circuit *b);
int getNumValid(std::vector<Circuit*> &circuits);

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
	std::vector<Point3d> points;
	std::vector<Distance> distances;
	std::vector<Circuit*> circuits;

	uint64_t sum = 1;

	while(std::getline(file, line))
	{
		points.push_back(parsePoint3d(splitString(line, ',')));
	}

	for(Point3d &pt : points)
	{
		Circuit *ct = new Circuit;
		pt.circuit = (void*) ct;
		ct->points.push_back(&pt);
		circuits.push_back(ct);
	}

	//calculate every distance
	//loop thorugh points
	for(int i = 0; i < points.size() - 1; i++)
	{
		//loop through points following the ith one.
		for(int j = i + 1; j < points.size(); j++)
		{
			distances.push_back(Distance(&points[i], &points[j], calcPoint3dDistance(points[i], points[j])));
		}
	}

	//sort distances in ascending order.
	std::sort(distances.begin(), distances.end(), compareDistance);
	//Loop through distances
	bool done = false;
	for(int i = 0; i < distances.size() && !done; i++)
	{
		// std::cout << "i = " << i << std::endl;
		// distances[i].disp();
		//if points are both in a different circuit.
		//combine circuits

		if(distances[i].pt_a->circuit != distances[i].pt_b->circuit)
			combineCircuit((Circuit *)distances[i].pt_a->circuit, (Circuit *)distances[i].pt_b->circuit);

		//if points are in the same circuit.
		//do nothing
		if(getNumValid(circuits) == 1)
		{
			done = true;
			sum = distances[i].pt_a->x * distances[i].pt_b->x;
		}
	}

	std::cout << "Total Sum = " << sum << std::endl;

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

Point3d parsePoint3d(std::vector<std::string> *pt_str)
{
	int64_t pts[3];
	for(int i = 0; i < pt_str->size(); i++)
	{
		pts[i] = std::stoll((*pt_str)[i], nullptr, 10);
	}

	return Point3d(pts[0], pts[1], pts[2]);
}

uint64_t calcPoint3dDistance(Point3d a, Point3d b)
{
	int64_t dx = a.x - b.x;
	int64_t dy = a.y - b.y;
	int64_t dz = a.z - b.z;

	return std::sqrt(dx*dx + dy*dy + dz*dz);
}

bool isPointEqual(Point3d *a, Point3d *b)
{
	return a->x == b->x && a->y == b->y && a->z == b->z;
}

bool compareDistance(const Distance &a, const Distance &b)
{
	return a.distance < b.distance;
}

void combineCircuit(Circuit *dest, Circuit *b)
{
	//loop through points in b add them to dest, and set their reference to dest;
	for(int i = 0; i < b->points.size(); i++)
	{
		bool in_dest = false;
		for(int j = 0; j < dest->points.size() && !in_dest; j++)
		{
			if(isPointEqual(dest->points[j], b->points[i]))
				in_dest = true;
		}
		if(!in_dest)
		{
			b->points[i]->circuit = (void*) dest;
			dest->points.push_back(b->points[i]);
		}
	}

	b->deleted = true;
}

int getNumValid(std::vector<Circuit*> &circuits)
{
	int num_valid = 0;
	for(Circuit *ct : circuits)
	{
		if(ct->deleted == false)
			num_valid++;
	}
	return num_valid;
}

