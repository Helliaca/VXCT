#include "Engine.h"

//Temporary references can be declared as global variables here
//Ex: glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

Scene* Engine::InitScene() {
	Scene* mainScene = new Scene();

	Model* sc_sphere1 = new Model("Sphere1", RenderShader::COLOR, OBJ_SCENE_SPHERE1);
	sc_sphere1->color = glm::vec3(0.5f, 0.5f, 0.5f);
	sc_sphere1->addMat4Reference("model_u", &sc_sphere1->model);
	sc_sphere1->addMat4Reference("proj_u", &G::SceneCamera->projMatrix);
	sc_sphere1->addMat4Reference("view_u", &G::SceneCamera->viewMatrix);
	sc_sphere1->addVec3Reference("lightPos", &G::lightPos);
	sc_sphere1->addVec3Reference("viewPos", &G::SceneCamera->Position);
	sc_sphere1->addVec3Reference("objectColor", &sc_sphere1->color);
	sc_sphere1->addVec3Reference("lightColor", &G::lightColor);
	mainScene->addObject(sc_sphere1);

	Model* sc_sphere2 = new Model("Sphere2", RenderShader::COLOR, OBJ_SCENE_SPHERE2);
	sc_sphere2->color = glm::vec3(0.5f, 0.5f, 0.5f);
	sc_sphere2->addMat4Reference("model_u", &sc_sphere2->model);
	sc_sphere2->addMat4Reference("proj_u", &G::SceneCamera->projMatrix);
	sc_sphere2->addMat4Reference("view_u", &G::SceneCamera->viewMatrix);
	sc_sphere2->addVec3Reference("lightPos", &G::lightPos);
	sc_sphere2->addVec3Reference("viewPos", &G::SceneCamera->Position);
	sc_sphere2->addVec3Reference("objectColor", &sc_sphere2->color);
	sc_sphere2->addVec3Reference("lightColor", &G::lightColor);
	mainScene->addObject(sc_sphere2);

	Model* sc_cube1 = new Model("Cube1", RenderShader::COLOR, OBJ_SCENE_CUBE1);
	sc_cube1->color = glm::vec3(0.5f, 0.5f, 0.5f);
	sc_cube1->addMat4Reference("model_u", &sc_cube1->model);
	sc_cube1->addMat4Reference("proj_u", &G::SceneCamera->projMatrix);
	sc_cube1->addMat4Reference("view_u", &G::SceneCamera->viewMatrix);
	sc_cube1->addVec3Reference("lightPos", &G::lightPos);
	sc_cube1->addVec3Reference("viewPos", &G::SceneCamera->Position);
	sc_cube1->addVec3Reference("objectColor", &sc_cube1->color);
	sc_cube1->addVec3Reference("lightColor", &G::lightColor);
	mainScene->addObject(sc_cube1);

	Model* sc_wall1 = new Model("Wall1", RenderShader::COLOR, OBJ_SCENE_WALL1);
	sc_wall1->color = glm::vec3(1.0f, 0.0f, 0.0f);
	sc_wall1->addMat4Reference("model_u", &sc_wall1->model);
	sc_wall1->addMat4Reference("proj_u", &G::SceneCamera->projMatrix);
	sc_wall1->addMat4Reference("view_u", &G::SceneCamera->viewMatrix);
	sc_wall1->addVec3Reference("lightPos", &G::lightPos);
	sc_wall1->addVec3Reference("viewPos", &G::SceneCamera->Position);
	sc_wall1->addVec3Reference("objectColor", &sc_wall1->color);
	sc_wall1->addVec3Reference("lightColor", &G::lightColor);
	mainScene->addObject(sc_wall1); //top

	Model* sc_wall2 = new Model("Wall2", RenderShader::COLOR, OBJ_SCENE_WALL2);
	sc_wall2->color = glm::vec3(1.0f, 0.0f, 0.0f);
	sc_wall2->addMat4Reference("model_u", &sc_wall2->model);
	sc_wall2->addMat4Reference("proj_u", &G::SceneCamera->projMatrix);
	sc_wall2->addMat4Reference("view_u", &G::SceneCamera->viewMatrix);
	sc_wall2->addVec3Reference("lightPos", &G::lightPos);
	sc_wall2->addVec3Reference("viewPos", &G::SceneCamera->Position);
	sc_wall2->addVec3Reference("objectColor", &sc_wall2->color);
	sc_wall2->addVec3Reference("lightColor", &G::lightColor);
	mainScene->addObject(sc_wall2); //back

	Model* sc_wall3 = new Model("Wall3", RenderShader::COLOR, OBJ_SCENE_WALL3);
	sc_wall3->color = glm::vec3(1.0f, 0.0f, 0.0f);
	sc_wall3->addMat4Reference("model_u", &sc_wall3->model);
	sc_wall3->addMat4Reference("proj_u", &G::SceneCamera->projMatrix);
	sc_wall3->addMat4Reference("view_u", &G::SceneCamera->viewMatrix);
	sc_wall3->addVec3Reference("lightPos", &G::lightPos);
	sc_wall3->addVec3Reference("viewPos", &G::SceneCamera->Position);
	sc_wall3->addVec3Reference("objectColor", &sc_wall3->color);
	sc_wall3->addVec3Reference("lightColor", &G::lightColor);
	mainScene->addObject(sc_wall3); //bottom

	Model* sc_wall4 = new Model("Wall4", RenderShader::COLOR, OBJ_SCENE_WALL4);
	sc_wall4->color = glm::vec3(1.0f, 0.0f, 0.0f);
	sc_wall4->addMat4Reference("model_u", &sc_wall4->model);
	sc_wall4->addMat4Reference("proj_u", &G::SceneCamera->projMatrix);
	sc_wall4->addMat4Reference("view_u", &G::SceneCamera->viewMatrix);
	sc_wall4->addVec3Reference("lightPos", &G::lightPos);
	sc_wall4->addVec3Reference("viewPos", &G::SceneCamera->Position);
	sc_wall4->addVec3Reference("objectColor", &sc_wall4->color);
	sc_wall4->addVec3Reference("lightColor", &G::lightColor);
	mainScene->addObject(sc_wall4); //front

	Model* sc_wall5 = new Model("Wall5", RenderShader::COLOR, OBJ_SCENE_WALL5);
	sc_wall5->color = glm::vec3(1.0f, 0.0f, 0.0f);
	sc_wall5->addMat4Reference("model_u", &sc_wall5->model);
	sc_wall5->addMat4Reference("proj_u", &G::SceneCamera->projMatrix);
	sc_wall5->addMat4Reference("view_u", &G::SceneCamera->viewMatrix);
	sc_wall5->addVec3Reference("lightPos", &G::lightPos);
	sc_wall5->addVec3Reference("viewPos", &G::SceneCamera->Position);
	sc_wall5->addVec3Reference("objectColor", &sc_wall5->color);
	sc_wall5->addVec3Reference("lightColor", &G::lightColor);
	mainScene->addObject(sc_wall5); //right

	Model* sc_wall6 = new Model("Wall6", RenderShader::COLOR, OBJ_SCENE_WALL6);
	sc_wall6->color = glm::vec3(1.0f, 0.0f, 0.0f);
	sc_wall6->addMat4Reference("model_u", &sc_wall6->model);
	sc_wall6->addMat4Reference("proj_u", &G::SceneCamera->projMatrix);
	sc_wall6->addMat4Reference("view_u", &G::SceneCamera->viewMatrix);
	sc_wall6->addVec3Reference("lightPos", &G::lightPos);
	sc_wall6->addVec3Reference("viewPos", &G::SceneCamera->Position);
	sc_wall6->addVec3Reference("objectColor", &sc_wall6->color);
	sc_wall6->addVec3Reference("lightColor", &G::lightColor);
	mainScene->addObject(sc_wall6); //left

	//sc_wall1->scale(0.5f);
	//sc_wall2->scale(0.5f);
	//sc_wall3->scale(0.5f);
	//sc_wall4->scale(0.5f);
	//sc_wall5->scale(0.5f);
	//sc_wall6->scale(0.5f);

	return mainScene;
}