#include "../Engine.h"

//Temporary references can be declared as global variables here
//Ex: glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

Scene* Engine::InitScene() {
	Scene* mainScene = new Scene();

	Model* sc_sphere1 = new Model("Sphere1", RenderShader::COLOR, OBJ_SCENE_SPHERE1);
	sc_sphere1->addMat4Reference("model_u", &sc_sphere1->model);
	sc_sphere1->addMat4Reference("proj_u", &G::SceneCamera->projMatrix);
	sc_sphere1->addMat4Reference("view_u", &G::SceneCamera->viewMatrix);
	sc_sphere1->addVec3Reference("viewPos", &G::SceneCamera->Position);
	sc_sphere1->addPlightReference("light", G::SceneLight);
	sc_sphere1->addMaterialReference("material", sc_sphere1->material);
	mainScene->addObject(sc_sphere1);

	Model* sc_sphere2 = new Model("Sphere2", RenderShader::COLOR, OBJ_SCENE_SPHERE2);
	sc_sphere2->addMat4Reference("model_u", &sc_sphere2->model);
	sc_sphere2->addMat4Reference("proj_u", &G::SceneCamera->projMatrix);
	sc_sphere2->addMat4Reference("view_u", &G::SceneCamera->viewMatrix);
	sc_sphere2->addVec3Reference("viewPos", &G::SceneCamera->Position);
	sc_sphere2->addPlightReference("light", G::SceneLight);
	sc_sphere2->addMaterialReference("material", sc_sphere2->material);
	mainScene->addObject(sc_sphere2);

	Model* sc_cube1 = new Model("Cube1", RenderShader::COLOR, OBJ_SCENE_CUBE1);
	sc_cube1->addMat4Reference("model_u", &sc_cube1->model);
	sc_cube1->addMat4Reference("proj_u", &G::SceneCamera->projMatrix);
	sc_cube1->addMat4Reference("view_u", &G::SceneCamera->viewMatrix);
	sc_cube1->addVec3Reference("viewPos", &G::SceneCamera->Position);
	sc_cube1->addPlightReference("light", G::SceneLight);
	sc_cube1->addMaterialReference("material", sc_cube1->material);
	//mainScene->addObject(sc_cube1);

	Model* sc_bunny = new Model("Bunny", RenderShader::COLOR, OBJ_SCENE_BUNNY);
	sc_bunny->addMat4Reference("model_u", &sc_bunny->model);
	sc_bunny->addMat4Reference("proj_u", &G::SceneCamera->projMatrix);
	sc_bunny->addMat4Reference("view_u", &G::SceneCamera->viewMatrix);
	sc_bunny->addVec3Reference("viewPos", &G::SceneCamera->Position);
	sc_bunny->addPlightReference("light", G::SceneLight);
	sc_bunny->addMaterialReference("material", sc_bunny->material);
	sc_bunny->setPosition(glm::vec3(0.0, -0.5, 0.0));
	//mainScene->addObject(sc_bunny);

	Model* sc_wall1 = new Model("Wall1", RenderShader::COLOR, OBJ_SCENE_WALL1);
	sc_wall1->addMat4Reference("model_u", &sc_wall1->model);
	sc_wall1->addMat4Reference("proj_u", &G::SceneCamera->projMatrix);
	sc_wall1->addMat4Reference("view_u", &G::SceneCamera->viewMatrix);
	sc_wall1->addVec3Reference("viewPos", &G::SceneCamera->Position);
	sc_wall1->addPlightReference("light", G::SceneLight);
	sc_wall1->material->shininess = 64.0f;
	sc_wall1->material->specular_str = 0.3f;
	sc_wall1->addMaterialReference("material", sc_wall1->material);
	mainScene->addObject(sc_wall1); //top

	Model* sc_wall2 = new Model("Wall2", RenderShader::COLOR, OBJ_SCENE_WALL2);
	sc_wall2->addMat4Reference("model_u", &sc_wall2->model);
	sc_wall2->addMat4Reference("proj_u", &G::SceneCamera->projMatrix);
	sc_wall2->addMat4Reference("view_u", &G::SceneCamera->viewMatrix);
	sc_wall2->addVec3Reference("viewPos", &G::SceneCamera->Position);
	sc_wall2->addPlightReference("light", G::SceneLight);
	sc_wall2->material->shininess = 64.0f;
	sc_wall2->material->specular_str = 0.3f;
	sc_wall2->addMaterialReference("material", sc_wall2->material);
	mainScene->addObject(sc_wall2); //back

	Model* sc_wall3 = new Model("Wall3", RenderShader::COLOR, OBJ_SCENE_WALL3);
	sc_wall3->addMat4Reference("model_u", &sc_wall3->model);
	sc_wall3->addMat4Reference("proj_u", &G::SceneCamera->projMatrix);
	sc_wall3->addMat4Reference("view_u", &G::SceneCamera->viewMatrix);
	sc_wall3->addVec3Reference("viewPos", &G::SceneCamera->Position);
	sc_wall3->addPlightReference("light", G::SceneLight);
	sc_wall3->material->shininess = 64.0f;
	sc_wall3->material->specular_str = 0.3f;
	sc_wall3->addMaterialReference("material", sc_wall3->material);
	mainScene->addObject(sc_wall3); //bottom

	Model* sc_wall4 = new Model("Wall4", RenderShader::COLOR, OBJ_SCENE_WALL4);
	sc_wall4->addMat4Reference("model_u", &sc_wall4->model);
	sc_wall4->addMat4Reference("proj_u", &G::SceneCamera->projMatrix);
	sc_wall4->addMat4Reference("view_u", &G::SceneCamera->viewMatrix);
	sc_wall4->addVec3Reference("viewPos", &G::SceneCamera->Position);
	sc_wall4->addPlightReference("light", G::SceneLight);
	sc_wall4->material->shininess = 64.0f;
	sc_wall4->material->specular_str = 0.3f;
	sc_wall4->addMaterialReference("material", sc_wall4->material);
	//mainScene->addObject(sc_wall4); //front

	Model* sc_wall5 = new Model("Wall5", RenderShader::COLOR, OBJ_SCENE_WALL5);
	sc_wall5->material->color = glm::vec3(0.4f, 1.0f, 0.4f);
	sc_wall5->addMat4Reference("model_u", &sc_wall5->model);
	sc_wall5->addMat4Reference("proj_u", &G::SceneCamera->projMatrix);
	sc_wall5->addMat4Reference("view_u", &G::SceneCamera->viewMatrix);
	sc_wall5->addVec3Reference("viewPos", &G::SceneCamera->Position);
	sc_wall5->addPlightReference("light", G::SceneLight);
	sc_wall5->material->shininess = 64.0f;
	sc_wall5->material->specular_str = 0.3f;
	sc_wall5->addMaterialReference("material", sc_wall5->material);
	mainScene->addObject(sc_wall5); //right

	Model* sc_wall6 = new Model("Wall6", RenderShader::COLOR, OBJ_SCENE_WALL6);
	sc_wall6->material->color = glm::vec3(1.0f, 0.4f, 0.4f);
	sc_wall6->addMat4Reference("model_u", &sc_wall6->model);
	sc_wall6->addMat4Reference("proj_u", &G::SceneCamera->projMatrix);
	sc_wall6->addMat4Reference("view_u", &G::SceneCamera->viewMatrix);
	sc_wall6->addVec3Reference("viewPos", &G::SceneCamera->Position);
	sc_wall6->addPlightReference("light", G::SceneLight);
	sc_wall6->material->shininess = 64.0f;
	sc_wall6->material->specular_str = 0.3f;
	sc_wall6->addMaterialReference("material", sc_wall6->material);
	mainScene->addObject(sc_wall6); //left

	return mainScene;
}