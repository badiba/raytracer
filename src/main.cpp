#include <iostream>
#include <vector>
#include <memory>
#include "render/scene.hpp"

int main(int argc, char* argv[])
{
    auto& scene = Scene::Instance();
    scene.Initialize(argv[1]);
    scene.Render();
}