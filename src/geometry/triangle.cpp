#include "shape.hpp"
#include <cstdio>

Triangle::Triangle(int id, int materialIndex, int point1Index, int point2Index, int point3Index)
    : Shape(id, materialIndex)
{
    this->point1Index = point1Index;
    this->point2Index = point2Index;
    this->point3Index = point3Index;
}

Intersection Triangle::intersect(const Ray& ray) const
{
	Intersection intersection;
    return intersection;
}
