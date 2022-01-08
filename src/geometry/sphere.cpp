#include "shape.hpp"
#include "../render/scene.hpp"
#include <cstdio>

using namespace Eigen;

Sphere::Sphere()
{

}

Sphere::Sphere(int id, int materialIndex, int centerIndex, float radius) : Shape(id, materialIndex)
{
    this->centerIndex = centerIndex;
    this->radius = radius;
}

Intersection Sphere::Intersect(const Ray& ray) const
{
    const auto& scene = Scene::Instance();
    Intersection intersection;

    const auto& d = ray.direction;
    const auto& o = ray.origin;
    const auto& c = scene.vertices[centerIndex - 1];
    float discriminant = ((d.dot(o - c)) * (d.dot(o - c)) - (d.dot(d)) * ((o - c).dot(o - c) - radius * radius));

    if (discriminant < scene.intTestEps)
    {
        intersection.intersected = false;
        return intersection;
    }

    float t1 = (-d.dot(o - c) + sqrt(discriminant)) / (d.dot(d));
    float t2 = (-d.dot(o - c) - sqrt(discriminant)) / (d.dot(d));
    Vector3f intersectionPoint;

    if (t2 > t1 && t2 > 0)
    {
        intersectionPoint = ray.GetPoint(t1);
    }
    else if (t1 > 0)
    {
        intersectionPoint = ray.GetPoint(t2);
    }
    else
    {
        intersection.intersected = false;
        return intersection;
    }

    intersection.point = intersectionPoint;
    intersection.normal = (intersectionPoint - c) / (intersectionPoint - c).norm();
    intersection.intersected = true;
    return intersection;
}
