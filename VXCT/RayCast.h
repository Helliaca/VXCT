#pragma once

#include "Model.h"

class RayCast : IOobject
{
public:
	RayCast(Model* model, glm::vec3 origin, glm::vec3 dir);
	~RayCast();
	bool execute(glm::vec3& loc_out, glm::vec3& nrm_out);
private:
	Model * model;
	glm::vec3 origin;
	glm::vec3 dir;
	bool TriIntersect(glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2, glm::vec3& out_pos, glm::vec3& out_nrm);
};

