#include "shape.hpp"
#include "../render/scene.hpp"
#include <cstdio>

using namespace Eigen;

Triangle::Triangle()
{

}

Triangle::Triangle(int id, int materialIndex, int point1Index, int point2Index, int point3Index) : Shape(id, materialIndex)
{
    this->point1Index = point1Index;
    this->point2Index = point2Index;
    this->point3Index = point3Index;
}

Intersection Triangle::Intersect(const Ray& ray) const
{
    auto& scene = Scene::Instance();
    Intersection intersection;

    const auto& a = scene.vertices[point1Index - 1];
    const auto& b = scene.vertices[point2Index - 1];
    const auto& c = scene.vertices[point3Index - 1];
    auto normal = (c - b).cross(a - b);

    Matrix3f matrix, matrixBeta, matrixGamma, matrixT;
    matrix << a - b, a - c, ray.direction;
    matrixBeta << a - ray.origin, a - c, ray.direction;
    matrixGamma << a - b, a - ray.origin, ray.direction;
    matrixT << a - b, a - c, a - ray.origin;

    auto det = matrix.determinant();
    auto beta = (matrixBeta).determinant() / (det);
    auto gamma = (matrixGamma).determinant() / (det);
    auto t = (matrixT).determinant() / (det);

    if (t >= -scene.intTestEps && (beta + gamma <= 1) && beta >= -scene.intTestEps && gamma >= -scene.intTestEps)
    {
        intersection.normal = normal / normal.norm();
        intersection.point = ray.GetPoint(t);
        intersection.intersected = true;
    }

    return intersection;
}
