#include "Scene.h"



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