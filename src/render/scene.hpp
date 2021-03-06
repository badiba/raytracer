#ifndef _SCENE_H_
#define _SCENE_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "ray.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "material.hpp"
#include "image.hpp"
#include "../geometry/shape.hpp"
#include "../defs.hpp"
#include "../dependencies/eigen-3.4.0/Eigen/Dense"
#include "../dependencies/tinyxml/tinyxml2.h"

class Scene
{
public:
	static Scene& Instance()
	{
		static Scene instance;
		return instance;
	}

	void Initialize(const char *xmlPath);
	void Render();

	Scene(Scene const&) = delete;
	void operator=(Scene const&) = delete;

	int maxRecursionDepth;
	float intTestEps;
	float shadowRayEps;
	Eigen::Vector3f backgroundColor;
	Eigen::Vector3f ambientLight;

	std::vector<Camera> cameras;
	std::vector<PointLight> lights;
	std::vector<Material> materials;
	std::vector<Eigen::Vector3f> vertices;
	std::vector<std::unique_ptr<Shape>> shapes;

private:
	Scene();
	bool IsShadow(const Eigen::Vector3f& point, const PointLight& light);
	Eigen::Vector3f GetSpecularContribution(const Ray& ray, const Intersection& intersection, const Material& mat, const PointLight& light);
	Eigen::Vector3f GetDiffuseContribution(const Intersection& intersection, const Material& mat, const Ray& ray, const PointLight& light);
	Eigen::Vector3f GetAmbientContribution(const Material& mat);
	Color GetShadingColor(const Ray& ray, const Intersection& intersection, const Material& mat);
};

#endif
