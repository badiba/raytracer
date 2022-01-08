#include "shape.hpp"
#include <cstdio>

Mesh::Mesh()
{

}

Mesh::Mesh(int id, int materialIndex, const std::vector<Triangle>& faces) : Shape(id, materialIndex)
{
    this->faces = faces;
}

Intersection Mesh::Intersect(const Ray& ray) const
{
	Intersection intersection;
    Intersection nearestIntersection;
    
    auto distance = 0.0f;
    auto nearestDistance = std::numeric_limits<float>::max();

    for (const auto& face : faces)
    {
        intersection = face.Intersect(ray);

        if (intersection.intersected)
        {
            distance = (intersection.point - ray.origin).norm();

            if (distance < nearestDistance)
            {
                nearestDistance = distance;
                nearestIntersection = intersection;
            }
        }
    }

    return nearestIntersection;
}
