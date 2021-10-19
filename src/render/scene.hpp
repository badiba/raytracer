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

	static void Initialize(const char *xmlPath);
	void RenderScene();

	Scene(Scene const&) = delete;
	void operator=(Scene const&) = delete;

	int max_recursion_depth_;
	float int_test_eps_;
	float shadow_ray_eps_;
	Eigen::Vector3f background_color_;
	Eigen::Vector3f ambient_light_;

	std::vector<std::shared_ptr<Camera>> cameras_;
	std::vector<std::shared_ptr<PointLight>> lights_;
	std::vector<std::shared_ptr<Material>> materials_;
	std::vector<Eigen::Vector3f> vertices_;
	std::vector<std::shared_ptr<Shape>> shapes_;

private:
    Scene() {}
};

#endif
