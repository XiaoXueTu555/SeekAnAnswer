#include "seek_math.h"

std::set<sint8> S_set_intersection(std::set<sint8> a, std::set<sint8> b)
{
	std::set<sint8> intersection;
	for (std::set<sint8>::iterator i = a.begin(); i != a.end(); i++)
	{
		if (b.find(*i) != b.end())
			intersection.insert(*i);
	}
	return intersection;
}