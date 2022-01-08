#include "scene.hpp"
#include "../dependencies/tinyxml/tinyxml2.h"
#include "../utils/parser.hpp"

using namespace Eigen;

bool Scene::IsShadow(const Vector3f& point, const PointLight& light)
{
    Intersection intersection;

    // Get direction vector from intersection to light.
    Vector3f direction = (light.position - point).normalized();

    // Create a new ray. Origin is moved with epsilon towards light to avoid self intersection.
    Ray ray(point + direction * shadowRayEps, direction);

    // Check intersection of the ray with all objects to see if there is a shadow.
    for (const auto& shape : shapes)
    {
        // Get intersection information.
        intersection = shape->Intersect(ray);

        if (intersection.intersected)
        {
            // Check if intersected object is between point and light.
            bool objectBlocksLight = (point - light.position).squaredNorm() > (point - intersection.point).squaredNorm();

            if (objectBlocksLight)
            {
                return true;
            }
        }
    }

    return false;
}

Vector3f Scene::GetSpecularContribution(const Ray& ray, const Intersection& intersection, const Material& mat, const PointLight& light)
{
    Vector3f wo = -ray.direction;
    Vector3f wi = (light.position - intersection.point).normalized();
    Vector3f h = (wo + wi).normalized();
    float nh = intersection.normal.dot(h);
    float alpha = std::max(0.0f, nh);

    return light.ComputeLightContribution(intersection.point).cwiseProduct(mat.specularRef * pow(alpha, mat.phongExp));
}

Vector3f Scene::GetDiffuseContribution(const Intersection& intersection, const Material& mat, const Ray& ray, const PointLight& light)
{
    Vector3f wi = (light.position - intersection.point).normalized();
    float nh = intersection.normal.dot(wi);
    float alpha = std::max(0.0f, nh);

    return (light.ComputeLightContribution(intersection.point).cwiseProduct(mat.diffuseRef * alpha));
}

Vector3f Scene::GetAmbientContribution(const Material& mat)
{
    return ambientLight.cwiseProduct(mat.ambientRef);
}

Color Scene::GetShadingColor(const Ray& ray, const Intersection& intersection, const Material& mat)
{
    // Compute ambient color (no shadow check).
    Vector3f rawColor = GetAmbientContribution(mat);

    // Check shadows for diffuse and specular shading (for every light source).
    for (const auto& light : lights)
    {
        if (!IsShadow(intersection.point, light))
        {
            // No shadow for this light: Add diffuse and specular shading color.
            rawColor += GetDiffuseContribution(intersection, mat, ray, light);
            rawColor += GetSpecularContribution(ray, intersection, mat, light);
        }
    }

    // Clamp to 255.
    rawColor = rawColor.cwiseMin(255);
    Color clampedColor{ (unsigned char)(rawColor(0)), (unsigned char)(rawColor(1)), (unsigned char)(rawColor(2)) };
    return clampedColor;
}

void Scene::Render()
{
	// Save an image for all cameras.
    for (const auto& camera : cameras)
    {
        Ray ray;
        int width = camera.imgPlane.nx;
        int height = camera.imgPlane.ny;
        Image image(width, height);

        // For every pixel create a ray.
        for (int i = 0; i < camera.imgPlane.nx; i++)
        {
            for (int j = 0; j < camera.imgPlane.ny; j++)
            {
                ray = camera.GetPrimaryRay(i, j);

                Intersection intersection;
                Intersection nearestIntersection;
                float intersectionDistance = 0.0f;
                float smallestIntersectionDistance = std::numeric_limits<float>::max();
                int nearestObjectIndex = 0;

                // Check intersection of the ray with all objects.
                for (int k = 0; k < shapes.size(); k++)
                {
                    auto& shape = shapes[k];
                    intersection = shape->Intersect(ray);
                    if (intersection.intersected)
                    {
                        // Save the nearest intersected object.
                        intersectionDistance = (intersection.point - ray.origin).squaredNorm();
                        if (intersectionDistance < smallestIntersectionDistance)
                        {
                            nearestObjectIndex = k;
                            smallestIntersectionDistance = intersectionDistance;
                            nearestIntersection = intersection;
                        }
                    }
                }

                // If intersected with an object, compute shading.
                if (nearestIntersection.intersected)
                {
                    auto& intersectedObject = shapes[nearestObjectIndex];
                    auto& material = materials[intersectedObject->materialIndex - 1];
                    image.SetPixelValue(i, j, GetShadingColor(ray, nearestIntersection, material));
                }
                // Else paint with background color.
                else
                {
                    image.SetPixelValue(i, j, 
                        Color{ 
                            (unsigned char)backgroundColor(0), 
                            (unsigned char)backgroundColor(1), 
                            (unsigned char)backgroundColor(2) 
                        });
                }
            }
        }

        image.SaveImage(camera.imageName);
    }
}

void Scene::Initialize(const char *xmlPath)
{
     auto& instance = Instance();

     // Set default values.
     instance.maxRecursionDepth = 1;
     instance.shadowRayEps = 0.001;

     // Load the xml file.
     tinyxml2::XMLDocument xmlDoc;
     xmlDoc.LoadFile(xmlPath);
     tinyxml2::XMLNode *pRoot = xmlDoc.FirstChild();

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

Scene::Scene()
{

}