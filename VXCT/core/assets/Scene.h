#pragma once

#include "Model.h"

class Scene : public IOobject
{
public:
	std::vector<Model*> objs;
	Scene();
	~Scene();
	void addObject(Model* obj);
	void draw();
	void draw(Shader* customShader);
	bool raycast(glm::vec3 origin, glm::vec3 dir, glm::vec3& out_pos, glm::vec3& out_nrm);
};

