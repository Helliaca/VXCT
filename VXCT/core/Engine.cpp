#include "Engine.h"

//#include <glad\glad.h>
//#include <GLFW\glfw3.h>

#include "shader.h"
#include "..\stb_image.h"
//#include "Camera.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

#include "..\VertexData.h"

//=====================================================================

//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//float lastX = WIN_WIDTH / 2.0f;
//float lastY = WIN_HEIGHT / 2.0f;
//bool firstMouse = true;

//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
float currentFrame = (float)glfwGetTime();

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

glm::mat4 viewMatrix;
glm::mat4 projMatrix;

const std::vector<GLubyte> texture3D(4 * VOX_SIZE * VOX_SIZE * VOX_SIZE, 0); //4 because RGBA

//=====================================================================


//Check how many vertex attributes our hardware allows
void shaderInfo() {
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
}

//=====================================================================

//load texture and stuff
unsigned int loadTexture(char const * path) {
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrChannels;
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data) {
		GLenum format;
		if (nrChannels == 1) format = GL_RED;
		else if (nrChannels == 3) format = GL_RGB;
		else if (nrChannels == 4) format = GL_RGBA;
		//bind to id
		glBindTexture(GL_TEXTURE_2D, textureID);
		//Hand image data to opengl
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		//Generate mipmaps for currently loaded texture
		glGenerateMipmap(GL_TEXTURE_2D);
		//set texture options
		//Wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//Filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//free image memory
		stbi_image_free(data);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}

//=====================================================================

Engine::Engine() : IOobject("unnamedEngine") {

}

Engine::~Engine() {

}

void Engine::loadGlad() { //only needs to be called once.
	//Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
}

void Engine::run() {
	window = new Window(); //create opengl context with GLFW

	loadGlad();	//Load Glad Extension library

	//This might not be necessary:
	//glEnable(GL_TEXTURE_3D);

	glEnable(GL_DEPTH_TEST); //turn on z-buffer
	glfwSetInputMode(window->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	shaderInfo(); //Display shader info

	//glm::mat4 viewMatrix; //These are global due to usage in voxelize()
	//glm::mat4 projMatrix; //^

	//==================================================================================


	//Model* mainCube = new Model("mainCube", RenderShader::VOX);		//Voxelization
	Model* mainCube = new Model("mainCube", RenderShader::COLOR);	//Regular Cube

	mainCube->addMat4Reference("model_u", &mainCube->model);
	mainCube->addMat4Reference("proj_u", &projMatrix);
	mainCube->addMat4Reference("view_u", &viewMatrix);
	mainCube->addVec3Reference("lightPos", &lightPos);
	mainCube->addVec3Reference("viewPos", &G::SceneCamera->Position);
	mainCube->addVec3Reference("objectColor", &glm::vec3(1.0f, 0.5f, 0.31f)); //Might lead to memory leaks as we keep no reference of this variable
	mainCube->addVec3Reference("lightColor", &glm::vec3(1.0f, 1.0f, 1.0f)); //Might lead to memory leaks as we keep no reference of this variable

	Model* lamp = new Model("lamp", RenderShader::EMIT);
	lamp->addMat4Reference("model", &lamp->model);
	lamp->addMat4Reference("view", &viewMatrix);
	lamp->addMat4Reference("projection", &projMatrix);
	lamp->addVec3Reference("emitColor", &glm::vec3(1.0f, 1.0f, 1.0f)); //Might lead to memory leaks as we keep no reference of this variable
	lamp->translate(lightPos);
	lamp->scale(0.2f); //A smaller cube

	Model* voxel = new Model("lamp", RenderShader::EMIT);
	voxel->addMat4Reference("model", &voxel->model);
	voxel->addMat4Reference("view", &viewMatrix);
	voxel->addMat4Reference("projection", &projMatrix);
	glm::vec3 putColorHere;
	voxel->addVec3Reference("emitColor", &putColorHere); //Might lead to memory leaks as we keep no reference of this variable
	voxel->scale(1.0f);

	std::string s;
	//Render loop:
	while (!window->shouldClose())
	{
		if(s!="free") std::cin >> s;
		if (s == "vox") {
			Voxelize();
			checkErrors();
			print(this, "Pause --");
			continue;
		}
		else checkErrors();

		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		G::deltaTime = currentFrame - G::lastFrame;
		G::lastFrame = currentFrame;

		//Update projection and view matrices
		projMatrix = glm::perspective(glm::radians(G::SceneCamera->Zoom), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
		viewMatrix = G::SceneCamera->GetViewMatrix();

		// input
		// -----
		window->processInput();

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lamp->draw();
		mainCube->draw();
		

		//window->setPolygonMode(PolygonMode::W_WIREFRAME);
		if (voxelMap != nullptr) {
			voxelMap->visualize(voxel, &putColorHere);
		}
		//window->setPolygonMode(PolygonMode::W_FILL);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window->getGLFWwindow());
		glfwPollEvents();
	}

	// de-allocate all resources once they've outlived their purpose:
	delete(lamp);
	delete(mainCube);

	glfwTerminate(); //Delete and clean all glfw resources allocated
	return;
}

void Engine::Voxelize() {
	//>>initVoxelization
	voxelMap = new VoxelMap(texture3D);

	projMatrix = glm::perspective(glm::radians(G::SceneCamera->Zoom), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
	viewMatrix = G::SceneCamera->GetViewMatrix();

	//Scene
	Model* mainCube = new Model("voxCube", RenderShader::VOX);	//Regular Cube
	mainCube->addMat4Reference("model_u", &mainCube->model);
	mainCube->addMat4Reference("proj_u", &projMatrix);
	mainCube->addMat4Reference("view_u", &viewMatrix);
	mainCube->addVec3Reference("lightPos", &lightPos);
	mainCube->addVec3Reference("viewPos", &G::SceneCamera->Position);
	mainCube->addVec3Reference("objectColor", &glm::vec3(1.0f, 0.5f, 0.31f)); //Might lead to memory leaks as we keep no reference of this variable
	mainCube->addVec3Reference("lightColor", &glm::vec3(1.0f, 1.0f, 1.0f)); //Might lead to memory leaks as we keep no reference of this variable

	//>>voxelize

	//if clearVoxelization: voxelMap->clear();
	mainCube->shader->use(); //needed here?
	glBindFramebuffer(GL_FRAMEBUFFER, 0); //TODO: why do we do this?

	//Settings
	glViewport(0, 0, VOX_SIZE, VOX_SIZE);
	//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); //?
	//glDisable(GL_CULL_FACE); //?
	//glDisable(GL_DEPTH_TEST); //?
	//glDisable(GL_BLEND); //?

	//Texture/Map
	voxelMap->activate(mainCube->shader->ID, "tex3D", 0);
	glBindImageTexture(0, voxelMap->textureID, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);
	

	// render
	// ------
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//mainCube->draw();
	mainCube->vox(voxelMap->textureID);

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// -------------------------------------------------------------------------------
	glfwSwapBuffers(window->getGLFWwindow());
	glfwPollEvents();

	//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); //if we set to false previously, revert.
	
	voxelMap->updateMemory();
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
}