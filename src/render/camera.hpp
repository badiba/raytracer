#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "ray.hpp"
#include "../defs.hpp"
#include "../dependencies/eigen-3.4.0/Eigen/Dense"

struct ImagePlane
{
	float left;
	float right;
	float bottom;
	float top;
    float distance;
    int nx;
    int ny;
};

class Camera
{
public:
    char imageName[32]; // TODO: Don't use char
    int id;
    ImagePlane imgPlane;

	Camera(int id, const char* imageName, const Eigen::Vector3f& pos, const Eigen::Vector3f& gaze, const Eigen::Vector3f& up, const ImagePlane& imgPlane);
	Ray GetPrimaryRay(int row, int col) const;

private:
    Eigen::Vector3f pos;
	Eigen::Vector3f gaze;
	Eigen::Vector3f up;
	Eigen::Vector3f right;
	float nxDivisionAvoided;
	float nyDivisionAvoided;
};

#endif
