#pragma once

#include "Model.h"

class Scene : IOobject
{
public:
	/*
	void addVec3Reference(std::string name, glm::vec3* ref) {
		for (std::vector<int>::size_type i = 0; i != objs.size(); i++) objs[i]->addVec3Reference(name, ref);
	}

	void addVec4Reference(std::string name, glm::vec4* ref) {
		for (std::vector<int>::size_type i = 0; i != objs.size(); i++) objs[i]->addVec4Reference(name, ref);
	}

	void addMat4Reference(std::string name, glm::mat4* ref) {
		for (std::vector<int>::size_type i = 0; i != objs.size(); i++) objs[i]->addMat4Reference(name, ref);
	}
	*/
	std::vector<Model*> objs;
	Scene();
	~Scene();
	void addObject(Model* obj);
	void draw();
	void vox();
};

