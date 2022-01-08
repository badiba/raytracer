#ifndef _PARSER_H_
#define _PARSER_H_

#include "../geometry/shape.hpp"
#include "../render/camera.hpp"
#include "../render/material.hpp"
#include "../dependencies/eigen-3.4.0/Eigen/Dense"
#include "tinyxml2.h"

namespace Parser
{
    void ParseIntegerProperty(tinyxml2::XMLNode* node, int& value, const char* propertyName)
    {
        auto pElement = node->FirstChildElement(propertyName);
		if (pElement != nullptr)
		{
			pElement->QueryIntText(&value);
		}
    }

    void ParseFloatProperty(tinyxml2::XMLNode* node, float& value, const char* propertyName)
    {
        auto pElement = node->FirstChildElement(propertyName);
		if (pElement != nullptr)
		{
			pElement->QueryFloatText(&value);
		}
    }

    void ParseVector3Property(tinyxml2::XMLNode* node, Eigen::Vector3f& value, const char* propertyName)
    {
        auto pElement = node->FirstChildElement(propertyName);
		auto str = pElement->GetText();
		sscanf_s(str, "%f %f %f", &value[0], &value[1], &value[2]);
    }

    void ParseCameras(tinyxml2::XMLNode* node, std::vector<Camera>& cameras)
    {
        auto pElement = node->FirstChildElement("Cameras");
		auto pCamera = pElement->FirstChildElement("Camera");
		tinyxml2::XMLElement* camElement;

        while (pCamera != nullptr)
		{
			int id;
			char imageName[64];
			Eigen::Vector3f pos, gaze, up;
			ImagePlane imgPlane;

			pCamera->QueryIntAttribute("id", &id);
            ParseVector3Property(pCamera, pos, "Position");
            ParseVector3Property(pCamera, gaze, "Gaze");
            ParseVector3Property(pCamera, up, "Up");

			camElement = pCamera->FirstChildElement("NearPlane");
			auto str = camElement->GetText();
			sscanf_s(str, "%f %f %f %f", &imgPlane.left, &imgPlane.right, &imgPlane.bottom, &imgPlane.top);

            ParseFloatProperty(pCamera, imgPlane.distance, "NearDistance");

			camElement = pCamera->FirstChildElement("ImageResolution");	
			str = camElement->GetText();
			sscanf_s(str, "%d %d", &imgPlane.nx, &imgPlane.ny);

			camElement = pCamera->FirstChildElement("ImageName");
			str = camElement->GetText();
			strcpy_s(imageName, str);

			cameras.push_back(Camera(id, imageName, pos, gaze, up, imgPlane));

			pCamera = pCamera->NextSiblingElement("Camera");
		}
    }

    void ParseMaterials(tinyxml2::XMLNode* node, std::vector<Material>& materials)
    {
        auto pElement = node->FirstChildElement("Materials");
		auto pMaterial = pElement->FirstChildElement("Material");
		tinyxml2::XMLElement* materialElement;

		while (pMaterial != nullptr)
		{
			materials.push_back(Material());
			int curr = materials.size() - 1;
		
			pMaterial->QueryIntAttribute("id", &materials[curr].id);
            ParseVector3Property(pMaterial, materials[curr].ambientRef, "AmbientReflectance");
            ParseVector3Property(pMaterial, materials[curr].diffuseRef, "DiffuseReflectance");
            ParseVector3Property(pMaterial, materials[curr].specularRef, "SpecularReflectance");

			materialElement = pMaterial->FirstChildElement("MirrorReflectance");
			if (materialElement != nullptr)
			{
				auto str = materialElement->GetText();
				sscanf_s(str, "%f %f %f", &materials[curr].mirrorRef[0], &materials[curr].mirrorRef[1], &materials[curr].mirrorRef[2]);
			}
			else
			{
				materials[curr].mirrorRef[0] = 0.0;
				materials[curr].mirrorRef[1] = 0.0;
				materials[curr].mirrorRef[2] = 0.0;
			}

			materialElement = pMaterial->FirstChildElement("PhongExponent");
			if(materialElement != nullptr)
            {
                materialElement->QueryIntText(&materials[curr].phongExp);
            }

			pMaterial = pMaterial->NextSiblingElement("Material");
		}
    }

	void ParseLights(tinyxml2::XMLNode* node, std::vector<PointLight>& lights, Eigen::Vector3f& ambientLight)
	{
		tinyxml2::XMLElement* lightElement;
		auto pElement = node->FirstChildElement("Lights");

		ParseVector3Property(pElement, ambientLight, "AmbientLight");

		auto pLight = pElement->FirstChildElement("PointLight");
		while (pLight != nullptr)
		{
			int id;
			Eigen::Vector3f position, intensity;

			pLight->QueryIntAttribute("id", &id);
			ParseVector3Property(pLight, position, "Position");
			ParseVector3Property(pLight, intensity, "Intensity");

			lights.push_back(PointLight(position, intensity));

			pLight = pLight->NextSiblingElement("PointLight");
		}
	}

    void ParseVertices(tinyxml2::XMLNode* node, std::vector<Eigen::Vector3f>& vertices)
    {
        auto pElement = node->FirstChildElement("VertexData");
		int cursor = 0;
		auto str = pElement->GetText();

		while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
        {
            cursor++;
        }
			
		while (str[cursor] != '\0')
		{
            Eigen::Vector3f point;

			for (int cnt = 0 ; cnt < 3 ; cnt++)
			{
				if (cnt == 0)
                {
                    point[0] = atof(str + cursor);
                }
				else if (cnt == 1)
                {
                    point[1] = atof(str + cursor);
                }
				else
                {
                    point[2] = atof(str + cursor);
                }

				while(str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
                {
                    cursor++; 
                }
					
				while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
                {
                    cursor++;
                }
			}

			vertices.push_back(point);
		}
    }

    void ParseShapes(tinyxml2::XMLNode* node, std::vector<std::unique_ptr<Shape>>& shapes)
    {
        auto pElement = node->FirstChildElement("Objects");
		
		// Parse spheres
		auto pObject = pElement->FirstChildElement("Sphere");
		tinyxml2::XMLElement* objElement;
		while (pObject != nullptr)
		{
			int id, matIndex, cIndex;
			float R;

			pObject->QueryIntAttribute("id", &id);
            ParseIntegerProperty(pObject, matIndex, "Material");
            ParseIntegerProperty(pObject, cIndex, "Center");
            ParseFloatProperty(pObject, R, "Radius");

			shapes.push_back(std::make_unique<Sphere>(id, matIndex, cIndex, R));
			pObject = pObject->NextSiblingElement("Sphere");
		}

		// Parse triangles
		pObject = pElement->FirstChildElement("Triangle");
		while (pObject != nullptr)
		{
			int id, matIndex, p1Index, p2Index, p3Index;

			pObject->QueryIntAttribute("id", &id);
            ParseIntegerProperty(pObject, matIndex, "Material");
			objElement = pObject->FirstChildElement("Indices");
			auto str = objElement->GetText();
			sscanf_s(str, "%d %d %d", &p1Index, &p2Index, &p3Index);

			shapes.push_back(std::make_unique<Triangle>(id, matIndex, p1Index, p2Index, p3Index));
			pObject = pObject->NextSiblingElement("Triangle");
		}

		// Parse meshes
		pObject = pElement->FirstChildElement("Mesh");
		while (pObject != nullptr)
		{
			int id, matIndex, p1Index, p2Index, p3Index, cursor = 0, vertexOffset = 0;
			std::vector<Triangle> faces;

			pObject->QueryIntAttribute("id", &id);
			ParseIntegerProperty(pObject, matIndex, "Material");
			objElement = pObject->FirstChildElement("Faces");
			objElement->QueryIntAttribute("vertexOffset", &vertexOffset);

			auto str = objElement->GetText();

			while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
			{
				cursor++;
			}
				
			while (str[cursor] != '\0')
			{
				for (int cnt = 0 ; cnt < 3 ; cnt++)
				{
					if (cnt == 0)
					{
						p1Index = atoi(str + cursor) + vertexOffset;
					}
					else if (cnt == 1)
					{
						p2Index = atoi(str + cursor) + vertexOffset;
					}
					else
					{
						p3Index = atoi(str + cursor) + vertexOffset;
					}

					while (str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
					{
						cursor++; 
					}
					
					while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
					{
						cursor++;
					}
				}

				faces.push_back(Triangle(-1, matIndex, p1Index, p2Index, p3Index));
			}

			shapes.push_back(std::make_unique<Mesh>(id, matIndex, faces));

			pObject = pObject->NextSiblingElement("Mesh");
		}
    }
}

#endif