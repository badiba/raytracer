#include "shape.hpp"
#include <cstdio>

Mesh::Mesh(int id, int materialIndex, const std::vector<Triangle>& faces)
    : Shape(id, materialIndex)
{
    this->faces = faces;
}

Intersection Mesh::intersect(const Ray& ray) const
{
	Intersection intersection;
    return intersection;
}
