#include "scene.hpp"
#include "../dependencies/tinyxml/tinyxml2.h"
#include "../utils/parser.hpp"

using namespace tinyxml2;
using namespace Eigen;

bool Scene::IsShadow(Vector3f point, const PointLight& light)
{
    auto intersection = Intersection();

    // Find direction vector from intersection to light.
    auto direction = light.position - point;

    // Create a new ray. Origin is moved with epsilon towards light to avoid self intersection.
    auto ray = Ray(point + direction * shadowRayEps, direction / direction.norm());

    // Check intersection of ray with all objects to see if there is a shadow.
    for (int k = 0; k < shapes.size(); k++)
    {
        // Get intersection information.
        intersection = shapes[k]->Intersect(ray);

        // Check if intersected object is between point and light.
        auto objectBlocksLight = (point - light.position).norm() > (point - intersection.point).norm();

        // If intersection happened and object is blocking the light then there is a shadow.
        if (intersection.intersected && objectBlocksLight)
        {
            return true;
        }
    }

    // No shadow, return false.
    return false;
}

Vector3f Scene::GetSpecularContribution(Ray ray, Intersection intersection, const Material& mat, const PointLight& light)
{
    // Compute specular color at given point with given light.
    auto wo = -ray.direction;
    auto wi = (light.position - intersection.point) / (light.position - intersection.point).norm();
    auto h = (wo + wi) / (wo + wi).norm();
    auto nh = intersection.normal.dot(h);
    auto alpha = std::max(0.0f, nh);

    auto specularColor = light.ComputeLightContribution(intersection.point).cwiseProduct(mat.specularRef * pow(alpha, mat.phongExp));
    return specularColor;
}

Vector3f Scene::GetDiffuseContribution(Intersection intersection, const Material& mat, Ray ray, const PointLight& light)
{
    // Compute diffuse color at given point with given light.
    auto wi = (light.position - intersection.point) / (light.position - intersection.point).norm();
    auto nh = intersection.normal.dot(wi);
    auto alpha = std::max(0.0f, nh);

    auto diffuseColor = (light.ComputeLightContribution(intersection.point).cwiseProduct(mat.diffuseRef * alpha));
    return diffuseColor;
}

Vector3f Scene::GetAmbientContribution(const Material& mat)
{
    // Create new ambient raw color (not bounded to 255).
    auto ambientColor = Vector3f(0, 0, 0);

    // Compute ambient color with given material coefficient.
    ambientColor += ambientLight.cwiseProduct(mat.ambientRef);
    return ambientColor;
}

Color Scene::GetShadingColor(Ray ray, Intersection intersection, const Material& mat)
{
    // Compute ambient color (no shadow check).
    auto rawColor = GetAmbientContribution(mat);

    // Check shadows for diffuse and specular shading (for every light source).
    for (auto i = 0; i < lights.size(); i++)
    {
        if (IsShadow(intersection.point, *lights[i]))
        {
            continue;
        }

        // No shadow for this light: Add diffuse and specular shading color.
        rawColor += GetDiffuseContribution(intersection, mat, ray, *lights[i]);
        rawColor += GetSpecularContribution(ray, intersection, mat, *lights[i]);
    }

    // Clamp and return.
    rawColor = rawColor.cwiseMin(255);
    auto clampedColor = Color{ (unsigned char)(rawColor(0)), (unsigned char)(rawColor(1)), (unsigned char)(rawColor(2)) };
    return clampedColor;
}

void Scene::Render()
{
	// Save an image for all cameras.
    for (auto i = 0; i < cameras.size(); i++)
    {
        auto cam = cameras[i];
        auto ray = Ray();
        auto width = cam->imgPlane.nx;
        auto height = cam->imgPlane.ny;
        auto image = Image(width, height);

        // For every pixel create a ray.
        for (auto i = 0; i < cam->imgPlane.nx; i++)
        {
            for (auto j = 0; j < cam->imgPlane.ny; j++)
            {
                ray = cam->GetPrimaryRay(i, j);

                auto intersection = Intersection();
                auto nearestIntersection = Intersection();
                auto returnDistance = 0.0f;
                auto nearestPoint = std::numeric_limits<float>::max();
                auto nearestObjectIndex = 0;

                // Check intersection of the ray with all objects.
                for (auto k = 0; k < shapes.size(); k++)
                {
                    intersection = shapes[k]->Intersect(ray);
                    if (intersection.intersected)
                    {
                        // Save the nearest intersected object.
                        returnDistance = (intersection.point - ray.origin).norm();
                        if (returnDistance < nearestPoint)
                        {
                            nearestObjectIndex = k;
                            nearestPoint = returnDistance;
                            nearestIntersection = intersection;
                        }
                    }
                }

                // If intersected with an object, compute shading.
                if (nearestIntersection.intersected)
                {
                    image.SetPixelValue(i, j, GetShadingColor(ray, nearestIntersection,
                            *materials[shapes[nearestObjectIndex]->materialIndex - 1]));
                }
                // Else paint with background color.
                else
                {
                    image.SetPixelValue(i, j, Color{ (unsigned char)backgroundColor(0),
                            (unsigned char)backgroundColor(1),
                            (unsigned char)backgroundColor(2) });
                }
            }
        }

        image.SaveImage(cam->imageName);
    }
}

void Scene::Initialize(const char *xmlPath)
{
     auto& instance = Instance();

     // Set default values.
     instance.maxRecursionDepth = 1;
     instance.shadowRayEps = 0.001;

     // Load the xml file.
     XMLDocument xmlDoc;
     xmlDoc.LoadFile(xmlPath);
     XMLNode *pRoot = xmlDoc.FirstChild();

     // Parse scene data.
     Parser::ParseIntegerProperty(pRoot, instance.maxRecursionDepth, "MaxRecursionDepth");
     Parser::ParseVector3Property(pRoot, instance.backgroundColor, "BackgroundColor");
     Parser::ParseFloatProperty(pRoot, instance.shadowRayEps, "ShadowRayEpsilon");
     Parser::ParseFloatProperty(pRoot, instance.intTestEps, "IntersectionTestEpsilon");
     Parser::ParseCameras(pRoot, instance.cameras);
     Parser::ParseMaterials(pRoot, instance.materials);
     Parser::ParseVertices(pRoot, instance.vertices);
     Parser::ParseShapes(pRoot, instance.shapes);
     Parser::ParseLights(pRoot, instance.lights, instance.ambientLight);
}