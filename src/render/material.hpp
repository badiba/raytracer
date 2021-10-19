#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "../defs.hpp"
#include "../dependencies/eigen-3.4.0/Eigen/Dense"

class Material
{
public:
	int id;
	int phongExp;
	Eigen::Vector3f ambientRef;
	Eigen::Vector3f diffuseRef;
	Eigen::Vector3f specularRef;
	Eigen::Vector3f mirrorRef;

	Material();
	
private:
	
};

#endif
