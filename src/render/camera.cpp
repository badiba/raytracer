#include "camera.hpp"

using namespace Eigen;

Camera::Camera(int id, const char* imageName, const Vector3f& pos, const Vector3f& gaze, const Vector3f& up, const ImagePlane& imgPlane)
{
    // TODO: Remove this keyword
	this->id = id;
	this->imgPlane = imgPlane;
	strcpy_s(this->imageName, imageName);

	this->gaze = gaze.normalized();
	auto w = (-gaze).normalized();
	this->right = (up.cross(w)).normalized();
	this->up = w.cross(right);
	this->pos = pos;

	nxDivisionAvoided = (float)1.0f / imgPlane.nx;
	nyDivisionAvoided = (float)1.0f / imgPlane.ny;
}

Ray Camera::GetPrimaryRay(int col, int row) const
{
	auto u = imgPlane.left + (imgPlane.right - imgPlane.left) * (col + 0.5f) * nxDivisionAvoided;
	auto v = imgPlane.top - (imgPlane.top - imgPlane.bottom) * (row + 0.5f) * nyDivisionAvoided;
	Vector3f m = pos + gaze * imgPlane.distance;
	m += u * right;
    m += v * up;
    return Ray(pos, (m - pos) / ((m - pos).norm()));
}
