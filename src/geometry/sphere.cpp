#include "shape.hpp"
#include "../render/scene.hpp"
#include <cstdio>

using namespace Eigen;

Sphere::Sphere()
    : Shape()
{

}

Sphere::Sphere(int id, int materialIndex, int centerIndex, float radius)
    : Shape(id, materialIndex)
{
    this->centerIndex = centerIndex;
    this->radius = radius;
}

Intersection Sphere::intersect(const Ray& ray) const
{
    /*
    const auto& scene = Scene::instance;
	const auto& d = ray.direction;
    const auto& o = ray.origin;
    const auto& c = scene->vertices[centerIndex - 1];

    Intersection intersection;

    float discriminant = ((d.dot(o - c)) * (d.dot(o - c)) - (d.dot(d)) * ((o - c).dot(o - c) - radius * radius));

    if (discriminant < scene->int_test_eps_)
    {
        intersection.full = false;
        return intersection;
    }

    float t1 = (-d.dot(o - c) + sqrt(discriminant)) / (d.dot(d));
    float t2 = (-d.dot(o - c) - sqrt(discriminant)) / (d.dot(d));

    Vector3f intersectionPoint;

    if (t1 >= 0 && t2 < 0)
    {
        intersectionPoint = ray.getPoint(t1);
    }
    else if (t2 >= 0 && t1 < 0)
    {
        intersectionPoint = ray.getPoint(t2);
    }
    else if (t1 < 0 && t2 < 0)
    {
        intersection.full = false;
        return intersection;
    }
    else
    {
        if (t1 < t2)
        {
            intersectionPoint = ray.getPoint(t1);
        }
        else
        {
            intersectionPoint = ray.getPoint(t2);
        }
    }

    intersection.point = intersectionPoint;
    intersection.normal = (intersectionPoint - c) / (intersectionPoint - c).norm(); // can be simplified
    intersection.full = true;

    return intersection;*/

    Intersection intersection;
    return intersection;
}
