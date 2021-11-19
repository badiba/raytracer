#ifndef _DEFS_H_
#define _DEFS_H_

#include "dependencies/eigen-3.4.0/Eigen/Dense"

struct Intersection
{
    bool intersected = false;
    Eigen::Vector3f point;
    Eigen::Vector3f normal;
};

#endif
