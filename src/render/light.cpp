#include "light.hpp"

using namespace Eigen;

PointLight::PointLight(const Vector3f & position, const Vector3f & intensity) : position(position), intensity(intensity)
{
}

Vector3f PointLight::ComputeLightContribution(const Vector3f& p) const
{
	return intensity / ((p - position).norm() * (p - position).norm());
}
