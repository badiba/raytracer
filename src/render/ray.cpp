#include <iostream>
#include "ray.hpp"

using namespace Eigen;

Ray::Ray()
{
}

Ray::Ray(const Vector3f& origin, const Vector3f& direction) : origin(origin), direction(direction)
{
}

Vector3f Ray::GetPoint(float t) const
{
	return origin + direction * t;
}

float Ray::GetT(const Eigen::Vector3f &p) const 
{
    // float == float returns false only if it is NaN

    auto tValue = (p[0] - origin[0]) / direction[0];
    if (tValue == tValue)
    {
        return tValue;
    }

    tValue = (p[1] - origin[1]) / direction[1];
    if (tValue == tValue)
    {
        return tValue;
    }

    tValue = (p[2] - origin[2]) / direction[2];
    if (tValue == tValue)
    {
        return tValue;
    }

    return 0;
}
