#include "scene.hpp"
#include "../dependencies/tinyxml/tinyxml2.h"
#include "../utils/parser.hpp"

using namespace tinyxml2;
using namespace Eigen;

void Scene::RenderScene()
{
	// TODO
}

void Scene::Initialize(const char *xmlPath)
{
     auto& instance = Instance();

     // Set default values.
     instance.max_recursion_depth_ = 1;
     instance.shadow_ray_eps_ = 0.001;

     // Load the xml file.
     XMLDocument xmlDoc;
     xmlDoc.LoadFile(xmlPath);
     XMLNode *pRoot = xmlDoc.FirstChild();

     // Parse scene data.
     Parser::ParseIntegerProperty(pRoot, instance.max_recursion_depth_, "MaxRecursionDepth");
     Parser::ParseVector3Property(pRoot, instance.background_color_, "BackgroundColor");
     Parser::ParseFloatProperty(pRoot, instance.shadow_ray_eps_, "ShadowRayEpsilon");
     Parser::ParseFloatProperty(pRoot, instance.int_test_eps_, "IntersectionTestEpsilon");
     Parser::ParseCameras(pRoot, instance.cameras_);
     Parser::ParseMaterials(pRoot, instance.materials_);
     Parser::ParseVertices(pRoot, instance.vertices_);
     Parser::ParseShapes(pRoot, instance.shapes_);
     Parser::ParseLights(pRoot, instance.lights_, instance.ambient_light_);
}