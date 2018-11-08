#include "Engine.h"

#include "..\stb_image.h"
#include <glm\ext.hpp>

//=====================================================================

float currentFrame = (float)glfwGetTime();

const std::vector<GLubyte> texture3D(4 * VOX_SIZE * VOX_SIZE * VOX_SIZE, 0); //4 because RGBA
const std::vector<GLubyte> texture3D_lod1(4 * VOX_SIZE * VOX_SIZE * VOX_SIZE, 0); //4 because RGBA
const std::vector<GLubyte> temporary_stuff(4 * VOX_SIZE * VOX_SIZE * VOX_SIZE, 0); //4 because RGBA

bool objs = true;					//Draw Objects
bool voxs = false;					//Draw Voxels  (Very costly! SfMode recommended)
bool voxelizeOnNextFrame = false;	//voxelize the Scene on the next Frame
bool rayOnNextFrame = false;		//Shoot raycast on next frame
bool show_detail_point = false;		//Show detailed info about ray_hit_point
bool voxsWireframe = false;			//Show voxels in Wireframe Mode
bool objsWireframe = false;			//Show objects in Wireframe Mode
bool sfMode = false;				//Display Frames individually on input
bool singleFrame = true;			//Show next frame
bool iLight = false;				//Toggle Indirect Light
bool overlayWireframe = false;		//Overlay objects with their Wireframe

glm::vec3 ray_hit_point = glm::vec3(0.0f);
glm::vec3 ray_hit_normal = glm::vec3(0.0f);

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

	Shader* voxIlluminShader = new Shader(VOXILLUMINSHADER_VS, VOXILLUMINSHADER_FS);

	mainScene = InitScene();

	DebugLine = new LineRenderer(); //Need to initiate this here because we need an opengl context

	Model* lamp = new Model("lamp", RenderShader::EMIT, defaultModels::cube_indices, defaultModels::cube_vertexData);
	lamp->addMat4Reference("model", &lamp->model);
	lamp->addMat4Reference("view", &G::SceneCamera->viewMatrix);
	lamp->addMat4Reference("projection", &G::SceneCamera->projMatrix);
	lamp->addVec3Reference("emitColor", &glm::vec3(1.0f, 1.0f, 1.0f)); //Might lead to memory leaks as we keep no reference of this variable
	lamp->translate(G::lightPos);
	lamp->scale(0.2f); //A smaller cube

	Model* voxel = new Model("voxel", RenderShader::EMIT, defaultModels::cube_indices, defaultModels::cube_vertexData);
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
		if (rayOnNextFrame) {
			glm::vec3 pos, nrm;
			if (this->mainScene->raycast(G::SceneCamera->Position, G::SceneCamera->Front, pos, nrm)) {
				print(this, "Ray position: " + glm::to_string(pos));
				ray_hit_point = pos;
				ray_hit_normal = nrm;
				print(this, "Ray normal: " + glm::to_string(nrm));
				show_detail_point = true;
			}
			else print(this, "no hit");
			rayOnNextFrame = false;
		}
		checkErrors("EngineLoop Pre-Render");

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
		if (objs) {
			if (iLight) {
				//voxelMap->activate(voxIlluminShader->ID, "tex3D", 0); //This line leads to errors, is it necessary?
				//glBindImageTexture(0, voxelMap->textureID, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8); //Is this necessary?
				mainScene->draw(voxIlluminShader);
			}
			else mainScene->draw();
			if (overlayWireframe) {
				window->setPolygonMode(PolygonMode::W_WIREFRAME);
				mainScene->draw();
				window->setPolygonMode(PolygonMode::W_FILL);
			}
		}
		if (objsWireframe) window->setPolygonMode(PolygonMode::W_FILL);

		if (show_detail_point) {
			voxel->setPosition(ray_hit_point);
			voxel->draw();
			DebugLine->drawLine(ray_hit_point, ray_hit_normal, 0.2f);
		}

		//TMP: Draw hit voxel in real time
		//glm::vec3 pos, nrm;
		//this->mainScene->raycast(G::SceneCamera->Position, G::SceneCamera->Front, pos, nrm);
		//tmp_ray_hit = pos;
		//voxel->setPosition(tmp_ray_hit);
		//voxel->draw();

		//Voxel Drawing
		if(voxsWireframe) window->setPolygonMode(PolygonMode::W_WIREFRAME);
		if (voxelMap != nullptr && voxs) {
			//voxelMap->visualize(voxel, &putColorHere); //Uncomment to re-enable voxel visualization.
			voxelMap_lod1->visualize(voxel, &putColorHere); //Uncomment to re-enable voxel visualization.
		}
		if(voxsWireframe) window->setPolygonMode(PolygonMode::W_FILL);


		settingMutex.unlock();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window->getGLFWwindow());
		glfwPollEvents();

		checkErrors("Engine Loop");
	}

	// de-allocate all resources once they've outlived their purpose:
	delete(lamp);
	//delete(mainCube);

	glfwTerminate(); //Delete and clean all glfw resources allocated
	return;
}

void Engine::Voxelize(Scene* scene) {
	//>>initVoxelization
	voxelMap = new VoxelMap(texture3D, temporary_stuff);
	voxelMap_lod1 = new VoxelMap(texture3D_lod1, temporary_stuff);

	G::SceneCamera->Update(); //Update view and projection matrices in SceneCamera before drawing anything

	Shader* sh = new Shader(VOXSHADER_VS, VOXSHADER_FS, VOXSHADER_GS);
	Shader* sh_lod1 = new Shader("../VXCT/shaders/tmp.vs", "../VXCT/shaders/tmp.fs", "../VXCT/shaders/tmp.gs");

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
	
	scene->draw(sh);

	// glfw: swap buffers
	// -------------------------------------------------------------------------------
	glfwSwapBuffers(window->getGLFWwindow());

	voxelMap->updateMemory();



	sh_lod1->use();
	glBindFramebuffer(GL_FRAMEBUFFER, 0); //TODO: why do we do this?
	glViewport(0, 0, VOX_SIZE, VOX_SIZE);
	glDisable(GL_CULL_FACE);

	voxelMap_lod1->activate(sh_lod1->ID, "tex3D", 0);
	glBindImageTexture(0, voxelMap_lod1->textureID, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);

	voxelMap->activate(sh_lod1->ID, "tex3D_in", 0);
	//glBindImageTexture(0, voxelMap->textureID, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);

	scene->draw(sh_lod1);



	//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); //if we set to false previously, revert.
	checkErrors("VoxelizeEnd");

	voxelMap_lod1->updateMemory();

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
		else if (input == "iLight") iLight = !iLight;
		else if (input == "overlayW") overlayWireframe = !overlayWireframe;
		else if (input == "pos1") G::SceneCamera->setPosition1();
		else if (input == "ray") rayOnNextFrame = true;
		else print(this, "Unknwon Command");

		//Calling checkErrors or similar methods here will lead to automatic exceptions because it does not share the OpenGL context of the main thread.

		settingMutex.unlock();
	}
}