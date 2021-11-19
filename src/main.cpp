#include <iostream>
#include <vector>
#include <memory>
#include "render/scene.hpp"

int main()
{
    auto& scene = Scene::Instance();
    scene.Initialize("E:\\raytracer\\scenes\\input01.xml");
    scene.Render();
}