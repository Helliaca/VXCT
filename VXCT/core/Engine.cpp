#include "Engine.h"

#include "shader.h"
#include "..\stb_image.h"

#include "..\VertexData.h"

//=====================================================================

float currentFrame = (float)glfwGetTime();

const std::vector<GLubyte> texture3D(4 * VOX_SIZE * VOX_SIZE * VOX_SIZE, 0); //4 because RGBA

bool objs = true;					//Draw Objects
bool voxs = true;					//Draw Voxels
bool voxelizeOnNextFrame = false;	//voxelize the Scene on the next Frame
bool voxsWireframe = false;			//Show voxels in Wireframe Mode
bool objsWireframe = false;			//Show objects in Wireframe Mode
bool sfMode = false;				//Display Frames individually on input
bool singleFrame = true;			//Show next frame

//=====================================================================


//Check how many vertex attributes our hardware allows
void shaderInfo() {
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
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

	if (VXCT_CULLING) {
		glEnable(GL_CULL_FACE); //Enable Face Culling
		glCullFace(GL_FRONT); //One sided faces
		glFrontFace(GL_CW); //Set front face to clockwise (GL_CCW for counterclockwise)
	}

	//==================================================================================

	Scene* mainScene = InitScene();

	Model* lamp = new Model("lamp", RenderShader::EMIT, defaultModels::cube_indices, defaultModels::cube_vertexData);
	lamp->addMat4Reference("model", &lamp->model);
	lamp->addMat4Reference("view", &G::SceneCamera->viewMatrix);
	lamp->addMat4Reference("projection", &G::SceneCamera->projMatrix);
	lamp->addVec3Reference("emitColor", &glm::vec3(1.0f, 1.0f, 1.0f)); //Might lead to memory leaks as we keep no reference of this variable
	lamp->translate(G::lightPos);
	lamp->scale(0.2f); //A smaller cube

	Model* voxel = new Model("lamp", RenderShader::EMIT, defaultModels::cube_indices, defaultModels::cube_vertexData);
	voxel->addMat4Reference("model", &voxel->model);
	voxel->addMat4Reference("view", &G::SceneCamera->viewMatrix);
	voxel->addMat4Reference("projection", &G::SceneCamera->projMatrix);
	glm::vec3 putColorHere; //The value of this vector will be changed in VoxelMap.visualize()
	voxel->addVec3Reference("emitColor", &putColorHere); //Might lead to memory leaks as we keep no reference of this variable
	voxel->scale((MAX_X - MIN_X)/VOX_SIZE); //Note: only MAX/MIN_X is taken into account when sclaing voxel representatives

	consoleThread = std::thread(&Engine::console, this);
	consoleThread.detach();

	//Render loop:
	while (!window->shouldClose())
	{
		settingMutex.lock();

		if (sfMode) {
			if (!singleFrame) {
				settingMutex.unlock();
				continue;
			}
			else singleFrame = false;
		}
		if (voxelizeOnNextFrame) {
			Voxelize(mainScene);
			checkErrors();
			voxelizeOnNextFrame = false;
		}
		checkErrors("EngineLoop");

		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		G::deltaTime = currentFrame - G::lastFrame;
		G::lastFrame = currentFrame;

		G::SceneCamera->Update(); //Update view and projection matrices in SceneCamera before drawing anything

		// input
		// -----
		if(!sfMode) window->processInput(); //Dont move Camera if in sfMode

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Object Drawing
		if (objsWireframe) window->setPolygonMode(PolygonMode::W_WIREFRAME);
		lamp->draw();
		if(objs) mainScene->draw();
		if (objsWireframe) window->setPolygonMode(PolygonMode::W_FILL);

		//Voxel Drawing
		if(voxsWireframe) window->setPolygonMode(PolygonMode::W_WIREFRAME);
		if (voxelMap != nullptr && voxs) {
			voxelMap->visualize(voxel, &putColorHere);
		}
		if(voxsWireframe) window->setPolygonMode(PolygonMode::W_FILL);


		settingMutex.unlock();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window->getGLFWwindow());
		glfwPollEvents();
	}

	// de-allocate all resources once they've outlived their purpose:
	delete(lamp);
	//delete(mainCube);

	glfwTerminate(); //Delete and clean all glfw resources allocated
	return;
}

void Engine::Voxelize(Scene* scene) {
	//>>initVoxelization
	voxelMap = new VoxelMap(texture3D);

	G::SceneCamera->Update(); //Update view and projection matrices in SceneCamera before drawing anything

	Shader* sh = new Shader(VOXSHADER_VS, VOXSHADER_FS, VOXSHADER_GS);

	checkErrors("VoxelizeInit");

	//>>voxelize

	//if clearVoxelization: voxelMap->clear();
	sh->use();
	glBindFramebuffer(GL_FRAMEBUFFER, 0); //TODO: why do we do this?

	//Settings
	glViewport(0, 0, VOX_SIZE, VOX_SIZE);
	//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); //?
	glDisable(GL_CULL_FACE);
	//glDisable(GL_DEPTH_TEST); //?
	//glDisable(GL_BLEND); //?

	//Texture/Map
	voxelMap->activate(sh->ID, "tex3D", 0);
	glBindImageTexture(0, voxelMap->textureID, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);
	
	checkErrors("VoxelizePreDraw");
	// render
	// ------
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	scene->vox();

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// -------------------------------------------------------------------------------
	glfwSwapBuffers(window->getGLFWwindow());
	glfwPollEvents();

	//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); //if we set to false previously, revert.
	checkErrors("VoxelizeEnd");

	voxelMap->updateMemory();

	//Revert Settings
	if(VXCT_CULLING) glEnable(GL_CULL_FACE);
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
}

void Engine::console() {
	std::string input;
	while (true)
	{
		std::cin >> input;

		settingMutex.lock();

		if (input == "objs") objs = !objs;
		else if (input == "voxs") voxs = !voxs;
		else if (input == "vox") voxelizeOnNextFrame = true;
		else if (input == "voxsW") voxsWireframe = !voxsWireframe;
		else if (input == "objsW") objsWireframe = !objsWireframe;
		else if (input == "sfMode") sfMode = !sfMode;
		else if (input == "sf" && sfMode) singleFrame = true;
		else print(this, "Unknwon Command");

		settingMutex.unlock();
	}
}