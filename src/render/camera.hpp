#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "ray.hpp"
#include "../defs.hpp"
#include "../dependencies/eigen-3.4.0/Eigen/Dense"

typedef struct ImagePlane
{
	float left;
	float right;
	float bottom;
	float top;
  float distance;
  int nx;
  int ny;
} ImagePlane;

class Camera
{
public:
  char imageName[32];
  int id;
  ImagePlane imgPlane;

	Camera(int id,
           const char* imageName,
           const Eigen::Vector3f& pos,
           const Eigen::Vector3f& gaze,
           const Eigen::Vector3f& up,
           const ImagePlane& imgPlane);

	Ray getPrimaryRay(int row, int col) const;

private:

};

#endif
