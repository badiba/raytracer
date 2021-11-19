#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "../defs.hpp"
#include "../dependencies/eigen-3.4.0/Eigen/Dense"

class PointLight
{
public:	
    Eigen::Vector3f position;

    PointLight(const Eigen::Vector3f& position, const Eigen::Vector3f& intensity);
    Eigen::Vector3f ComputeLightContribution(const Eigen::Vector3f& p) const;

private:
    Eigen::Vector3f intensity;
};

#endif
