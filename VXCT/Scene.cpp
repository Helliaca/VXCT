#include "Scene.h"

#include "RayCast.h"

#include "VertexData.h"

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
		objs[i]->draw(customShader);
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
			print(this, "Raycast hit object: " + objs[i]->name);
			if (!hit || glm::distance(origin, pos) < glm::distance(origin, closestPos)) {
				closestPos = pos;
				closestPos_nrm = nrm;
			}
			hit = true;
		}
	}
	if (hit) {
		out_pos = pos;
		out_nrm = nrm;
		return true;
	}
	return false;
}