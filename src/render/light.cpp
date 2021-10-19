#include "light.hpp"

using namespace Eigen;

PointLight::PointLight(const Vector3f & position, const Vector3f & intensity)
    : position(position), intensity(intensity)
{
}

Vector3f PointLight::computeLightContribution(const Vector3f& p)
{
	// TODO
}
