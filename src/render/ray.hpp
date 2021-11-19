#ifndef _RAY_H_
#define _RAY_H_

#include "../defs.hpp"
#include "../dependencies/eigen-3.4.0/Eigen/Dense"

class Ray
{
public:
	Eigen::Vector3f origin;
	Eigen::Vector3f direction;

	Ray();
	Ray(const Eigen::Vector3f& origin, const Eigen::Vector3f& direction);

	Eigen::Vector3f GetPoint(float t) const;
	float GetT(const Eigen::Vector3f& p) const;

private:

};

#endif
