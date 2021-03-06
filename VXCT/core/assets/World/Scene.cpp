#include "Scene.h"

#include "../Utilities/RayCast.h"

Scene::Scene() : IOobject("unnamedScene")
{
}


Scene::~Scene()
{
}

void Scene::addObject(Model* obj) {
	objs.push_back(obj);
}

void Scene::draw() {
	for (std::vector<int>::size_type i = 0; i != objs.size(); i++) {
		objs[i]->draw();
	}
}

void Scene::draw(Shader* customShader) {
	for (std::vector<int>::size_type i = 0; i != objs.size(); i++) {
		if(!objs[i]->is_static) objs[i]->draw(customShader);
		else objs[i]->draw();
	}
}

bool Scene::raycast(glm::vec3 origin, glm::vec3 dir, glm::vec3& out_pos, glm::vec3& out_nrm) {
	glm::vec3 closestPos;
	glm::vec3 closestPos_nrm;
	glm::vec3 pos;
	glm::vec3 nrm;
	bool hit = false;

	for (std::vector<int>::size_type i = 0; i != objs.size(); i++) {
		print(this, "Raycasting onto object: " + objs[i]->name);
		RayCast rc = RayCast(objs[i], origin, dir);

		if (rc.execute(pos, nrm)) {
			print(this, "Raycast hit object: " + objs[i]->name + ", Distance: " + std::to_string(glm::distance(origin, pos)));
			if (!hit || glm::distance(origin, pos) < glm::distance(origin, closestPos)) {
				closestPos = pos;
				closestPos_nrm = nrm;
			}
			hit = true;
		}
	}
	if (hit) {
		out_pos = closestPos;
		out_nrm = closestPos_nrm;
		return true;
	}
	return false;
}