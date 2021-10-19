#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "../defs.hpp"
#include "../render/ray.hpp"

class Shape
{
public:
    Shape();
    Shape(int id, int materialIndex);
    virtual Intersection intersect(const Ray& ray) const = 0;

protected:
    int id;
    int materialIndex;

private:
    
};

class Sphere: public Shape
{
public:
    Sphere();
	Sphere(int id, int materialIndex, int centerIndex, float radius);
	Intersection intersect(const Ray& ray) const;

private:
    int centerIndex;
	float radius;
};

class Triangle : public Shape
{
public:
    Triangle();
	Triangle(int id, int materialIndex, int point1Index, int point2Index, int point3Index);
	Intersection intersect(const Ray& ray) const;

private:
	int point1Index;
    int point2Index;
    int point3Index;
};

class Mesh: public Shape
{
public:
    Mesh();
	Mesh(int id, int materialIndex, const std::vector<Triangle>& faces);
	Intersection intersect(const Ray& ray) const;

private:
	std::vector<Triangle> faces;
};



#endif