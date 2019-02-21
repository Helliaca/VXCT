#include "Engine.h"

//#include "../stb_image.h"
#include <glm/ext.hpp>
#include <iterator>

//=====================================================================

float currentFrame = (float)glfwGetTime();


// These variables are manipulated by the console() thread and determine the output accordingly
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
bool LocLod = false;				//Toggle Localized LOD mode
bool overlayWireframe = false;		//Overlay objects with their Wireframe
int drawLod = 0;					//LOD level to draw mipmaps of
bool loadSceneOnNextFrame = false;	//Load a Scene next frame
bool dynamic_scene = false;			//Revoxelize after every voxelize_freq time
float voxelize_freq = 0.5;			//Revoxelization frequency for dynamic scenes
std::string scene_load_dir = "";	//Scene File path to load if the value above is true

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

Engine::Engine() : IOobject("mainEngine") {

}

Engine::~Engine() {

}

void Engine::loadGlad() { //only needs to be called once.
	//Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		print(this, "Failed to initialize GLAD");
		return;
	}
}

void Engine::run() {
	 
	// >> Create Context and set settings <<

	window = new Window();
	loadGlad();

	glEnable(GL_DEPTH_TEST); //turn on z-buffer
	glEnable(GL_BLEND);		 //turn on blending for transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //choose blending function
	
	glfwSetInputMode(window->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	shaderInfo(); //Display shader info

	if (VXCT_CULLING) {
		glEnable(GL_CULL_FACE); //Enable Face Culling
		glCullFace(GL_FRONT); //One sided faces
		glFrontFace(GL_CW); //Set front face to clockwise (GL_CCW for counterclockwise)
	}

	frametimecounter = new FrameTimeCounter(FRAMETIMES_SAVE);
	revox_timer = new Timer();

	// >> Initialize Attribute Objects <<

	Shader* voxIlluminShader = new Shader(VOXILLUMINSHADER_VS, VOXILLUMINSHADER_FS);
	Shader* locLodShader = new Shader(LODTEXTURESHADER_VS, LODTEXTURESHADER_FS);
	voxelization_shader = new Shader(VOXSHADER_VS, VOXSHADER_FS, VOXSHADER_GS);

	SceneParser* sceneParser = new SceneParser();
	sceneParser->parse(SCENE_TXT);
	mainScene = sceneParser->to_scene();

	DebugLine = new LineRenderer();

	Model* voxel = new Model("voxel", RenderShader::EMIT, defaultModels::cube_indices, defaultModels::cube_vertexData);
	voxel->addMat4Reference("model_u", &voxel->model);
	voxel->addMat4Reference("view_u", &G::SceneCamera->viewMatrix);
	voxel->addMat4Reference("proj_u", &G::SceneCamera->projMatrix);
	glm::vec3 putColorHere; //The value of this vector will be changed in VoxelMap.visualize()
	voxel->addVec3Reference("material.color", &putColorHere); //Might lead to memory leaks as we keep no reference of this variable

	voxelMap = new VoxelMap();

	// >> Restart Timers and separate cmd <<

	frametimecounter->start();
	revox_timer->start();

	consoleThread = std::thread(&Engine::console, this);
	consoleThread.detach();

	// == TEMPORARY FIX ==
	// For some reason compiling this program with /MD instead of /MDd leads to only these values being false. I know it makes no sense, but this at leas fixes it...
	G::VoxLightSettings->intermediate_cones = true;
	G::VoxLightSettings->front_cone = true;
	G::VoxLightSettings->side_cones = false;

	// >> Render Loop <<

	while (!window->shouldClose())
	{
		settingMutex.lock();

		// > Pre-Render <

		// Single-Frame mode
		if (sfMode) {
			if (!singleFrame) {
				settingMutex.unlock();
				continue;
			}
			else singleFrame = false;
		}
		// Voxelize on this frame
		if (voxelizeOnNextFrame || (dynamic_scene && revox_timer->elapsedTime()>voxelize_freq)) {
			Voxelize(mainScene);
			checkErrors();
			voxelizeOnNextFrame = false;
			revox_timer->start();
		}
		// Perform a raycast
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
		// Load a scene on this frame
		if (loadSceneOnNextFrame) {
			SceneParser* sp = new SceneParser();
			if(sp->parse(scene_load_dir)) mainScene = sp->to_scene();
			loadSceneOnNextFrame = false;
		}
		checkErrors("EngineLoop Pre-Render");

		// > Per Frame Logic <
		float currentFrame = glfwGetTime();
		G::deltaTime = currentFrame - G::lastFrame;
		G::lastFrame = currentFrame;

		if(!sfMode) window->processInput(); //Dont move Camera if in sfMode

		// > Render <

		//Clear canvas
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw objects/models
		if (objsWireframe) window->setPolygonMode(PolygonMode::W_WIREFRAME); //Wireframe mode
		if (objs) {
			if (LocLod) {
				locLodShader->use();
				locLodShader->setInt("lod_level", drawLod);
				mainScene->draw(locLodShader);
			}
			else if (iLight) {
				mainScene->draw(voxIlluminShader);
			}
			else if (overlayWireframe) {
				window->setPolygonMode(PolygonMode::W_WIREFRAME);
				mainScene->draw();
				window->setPolygonMode(PolygonMode::W_FILL);
			}
			else mainScene->draw();
		}
		if (objsWireframe) window->setPolygonMode(PolygonMode::W_FILL);

		if (show_detail_point) {
			DebugLine->drawLine(ray_hit_point, ray_hit_normal, 0.2f);
		}

		// Draw Voxels
		if(voxsWireframe) window->setPolygonMode(PolygonMode::W_WIREFRAME);
		if (voxelMap != nullptr && voxs) {
			voxelMap->visualize(voxel, &putColorHere, drawLod);
		}
		if(voxsWireframe) window->setPolygonMode(PolygonMode::W_FILL);


		settingMutex.unlock();

		// > Swap Buffers and poll IO <
		glfwSwapBuffers(window->getGLFWwindow());
		glfwPollEvents();

		//glFinish(); //use this to measure frame-times

		frametimecounter->nextFrame();

		checkErrors("Engine Loop");
	}

	glfwTerminate(); //Delete and clean all glfw resources allocated
	return;
}

void Engine::Voxelize(Scene* scene) {
	//>>voxelize
	
	voxelMap->clear();
	voxelization_shader->use();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	checkErrors("VoxelizeInit");

	//Settings
	glViewport(0, 0, VOX_SIZE, VOX_SIZE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	
	//Texture/Map
	voxelMap->use(voxelization_shader->ID, "tex3D", 0);
	glBindImageTexture(0, voxelMap->textureID, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);
	
	checkErrors("VoxelizePreDraw");

	//Voxelize	
	scene->draw(voxelization_shader);

	//glfwSwapBuffers(window->getGLFWwindow());

	voxelMap->updateMemory(!dynamic_scene);

	//Revert Settings
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	if(VXCT_CULLING) glEnable(GL_CULL_FACE);
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
}

void Engine::console() {
	std::string in;
	while (true)
	{
		std::getline(std::cin, in);
		std::istringstream iss(in);
		std::vector<std::string> input(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());

		settingMutex.lock();

		if (input.size() == 2) {
			if (input[0] == "diffuse_dist_factor" || input[0] == "ddf") { G::VoxLightSettings->diffuse_dist_factor = strtof(input[1].c_str(), 0); }
			else if (input[0] == "occlusion_dist_factor" || input[0] == "odf") { G::VoxLightSettings->occlusion_dist_factor = strtof(input[1].c_str(), 0); }
			else if (input[0] == "specular_dist_factor" || input[0] == "sdf") { G::VoxLightSettings->specular_dist_factor = strtof(input[1].c_str(), 0); }

			else if (input[0] == "diffuse_offset" || input[0] == "do") { G::VoxLightSettings->diffuse_offset = strtof(input[1].c_str(), 0); }
			else if (input[0] == "diffuse_origin_offset" || input[0] == "doo") { G::VoxLightSettings->diffuse_origin_offset = strtof(input[1].c_str(), 0); }
			else if (input[0] == "occlusion_offset" || input[0] == "oo") { G::VoxLightSettings->occlusion_offset = strtof(input[1].c_str(), 0); }
			else if (input[0] == "specular_offset" || input[0] == "so") { G::VoxLightSettings->specular_offset = strtof(input[1].c_str(), 0); }

			else if (input[0] == "diffuse_aperture" || input[0] == "da") { G::VoxLightSettings->diffuse_apperture = strtof(input[1].c_str(), 0); }
			else if (input[0] == "occlusion_aperture" || input[0] == "oa") { G::VoxLightSettings->occlusion_apperture = strtof(input[1].c_str(), 0); }
			else if (input[0] == "specular_aperture" || input[0] == "sa") { G::VoxLightSettings->specular_apperture = strtof(input[1].c_str(), 0); }

			else if (input[0] == "shadow_str" || input[0] == "ss") { G::VoxLightSettings->shadow_str = strtof(input[1].c_str(), 0); }
			else if (input[0] == "shininess_falloff" || input[0] == "sf") { G::VoxLightSettings->shininess_falloff = strtof(input[1].c_str(), 0); }

			else if (input[0] == "lod") { drawLod = (int)strtof(input[1].c_str(), 0); }
			else if (input[0] == "lastframes") { frametimecounter->printLastFrames(strtof(input[1].c_str(), 0)); }

			else if (input[0] == "load") { loadSceneOnNextFrame = true; scene_load_dir = SCENE_DIR + input[1] + ".txt"; }
			else if (input[0] == "vox_freq") { voxelize_freq = strtof(input[1].c_str(), 0); }
			else if (input[0] == "toggle") {
				for (int i = 0; i < mainScene->objs.size(); i++) {
					if (mainScene->objs[i]->name == input[1]) toggle(&mainScene->objs[i]->active, mainScene->objs[i]->name);
				}
			}
			else print(this, "Unknwon Command");
		}
		else if (input.size() == 3) {
			if (input[0] == "scale") {
				for (int i = 0; i < mainScene->objs.size(); i++) {
					if (mainScene->objs[i]->name == input[1]) {
						float scalar = strtof(input[2].c_str(), 0);
						mainScene->objs[i]->scale( glm::vec3(scalar) );
					}
				}
			}
			else print(this, "Unknwon Command");
		}
		//eg. setmat Sphere1 shininess 0.1
		else if (input.size() == 4) {
			if (input[0] == "setmat") {
				for (int i = 0; i < mainScene->objs.size(); i++) {
					if (mainScene->objs[i]->name == input[1]) {
						if(input[2] == "shininess") { mainScene->objs[i]->material->shininess = strtof(input[3].c_str(), 0); }
						if (input[2] == "ambient_str") { mainScene->objs[i]->material->ambient_str = strtof(input[3].c_str(), 0); }
						if (input[2] == "diffuse_str") { mainScene->objs[i]->material->diffuse_str = strtof(input[3].c_str(), 0); }
						if (input[2] == "specular_str") { mainScene->objs[i]->material->specular_str = strtof(input[3].c_str(), 0); }
					}
				}
			}
			else if (input[0] == "translate") {
				for (int i = 0; i < mainScene->objs.size(); i++) {
					if (mainScene->objs[i]->name == input[1]) {
						if (input[2] == "x") { mainScene->objs[i]->translate( strtof(input[3].c_str(), 0), 0, 0); }
						else if (input[2] == "y") { mainScene->objs[i]->translate(0, strtof(input[3].c_str(), 0), 0); }
						else if (input[2] == "z") { mainScene->objs[i]->translate(0, 0, strtof(input[3].c_str(), 0)); }
						else print(this, "Invalid Axis");
					}
				}
			}
			else if (input[0] == "rotate") {
				for (int i = 0; i < mainScene->objs.size(); i++) {
					if (mainScene->objs[i]->name == input[1]) {
						if (input[2] == "x") { mainScene->objs[i]->rotate(glm::vec3(strtof(input[3].c_str(), 0), 0, 0)); }
						else if (input[2] == "y") { mainScene->objs[i]->rotate(glm::vec3(0, strtof(input[3].c_str(), 0), 0)); }
						else if (input[2] == "z") { mainScene->objs[i]->rotate(glm::vec3(0, 0, strtof(input[3].c_str(), 0))); }
						else print(this, "Invalid Axis");
					}
				}
			}
			else print(this, "Unknwon Command");
		}
		else  if (input.size() == 1) {
			if (input[0] == "objs") { toggle(&objs, "Draw Objects"); }
			else if (input[0] == "voxs") { toggle(&voxs, "Draw Voxels"); }
			else if (input[0] == "vox") voxelizeOnNextFrame = true;
			else if (input[0] == "voxsW") { toggle(&voxsWireframe, "Wireframe Voxels"); }
			else if (input[0] == "objsW") { toggle(&objsWireframe, "Wireframe Objects"); }
			else if (input[0] == "sfMode") { toggle(&sfMode, "Single Frame Mode"); }
			else if (input[0] == "sf" && sfMode) singleFrame = true;
			else if (input[0] == "iLight") { toggle(&iLight, "VXCT"); }
			else if (input[0] == "overlayW") { toggle(&overlayWireframe, "Wireframe Overlay"); }
			else if (input[0] == "pos1") G::SceneCamera->setPosition(1);
			else if (input[0] == "pos2") G::SceneCamera->setPosition(2);
			else if (input[0] == "ray") rayOnNextFrame = true;
			else if (input[0] == "loclod") { toggle(&LocLod, "Localized LOD Mode"); }
			else if (input[0] == "avgf") frametimecounter->printAvg();
			else if (input[0] == "clearf") frametimecounter->clear();
			else if (input[0] == "dynamic") { toggle(&dynamic_scene, "Dynamic Revoxelization"); }

			else if (input[0] == "phong") toggle(&G::VoxLightSettings->phong, "VXCT Phong");
			else if (input[0] == "phong_ambient") toggle(&G::VoxLightSettings->phong_ambient, "VXCT Phong Ambient");
			else if (input[0] == "phong_diffuse") toggle(&G::VoxLightSettings->phong_diffuse, "VXCT Phong Diffuse");
			else if (input[0] == "phong_specular") toggle(&G::VoxLightSettings->phong_specular, "VXCT Phong Specular");

			else if (input[0] == "front_cone") toggle(&G::VoxLightSettings->front_cone, "VXCT Front Cone");
			else if (input[0] == "side_cones") toggle(&G::VoxLightSettings->side_cones, "VXCT Side Cones");
			else if (input[0] == "intermediate_cones") toggle(&G::VoxLightSettings->intermediate_cones, "VXCT Intermediate Cones");

			else if (input[0] == "vox_diffuse" || input[0] == "vdiff") toggle(&G::VoxLightSettings->vox_diffuse, "VXCT Diffuse Component");
			else if (input[0] == "vox_specular" || input[0] == "vspec") toggle(&G::VoxLightSettings->vox_specular, "VXCT Specular Component");
			else if (input[0] == "vox_shadows" || input[0] == "vshad") toggle(&G::VoxLightSettings->vox_shadows, "VXCT Occlusion Component");

			else if (input[0] == "phong_only") { G::VoxLightSettings->vox_diffuse = G::VoxLightSettings->vox_shadows = G::VoxLightSettings->vox_specular = false; G::VoxLightSettings->phong = true; }
			else if (input[0] == "shadows_only") { G::VoxLightSettings->vox_diffuse = G::VoxLightSettings->phong = G::VoxLightSettings->vox_specular = false; G::VoxLightSettings->vox_shadows = true; }
			else if (input[0] == "specular_only") { G::VoxLightSettings->vox_diffuse = G::VoxLightSettings->vox_shadows = G::VoxLightSettings->phong = false; G::VoxLightSettings->vox_specular = true; }
			else if (input[0] == "diffuse_only") { G::VoxLightSettings->phong = G::VoxLightSettings->vox_shadows = G::VoxLightSettings->vox_specular = false; G::VoxLightSettings->vox_diffuse = true; }
			else if (input[0] == "all") { G::VoxLightSettings->phong = G::VoxLightSettings->vox_shadows = G::VoxLightSettings->vox_specular = G::VoxLightSettings->vox_diffuse = true; }

			else if (input[0] == "diffuse_offset" || input[0] == "do") { print(this, "Diffuse Offset : " + std::to_string(G::VoxLightSettings->diffuse_offset)); }
			else if (input[0] == "occlusion_offset" || input[0] == "oo") { print(this, "Occlusion Offset : " + std::to_string(G::VoxLightSettings->occlusion_offset)); }
			else if (input[0] == "specular_offset" || input[0] == "so") { print(this, "Specular Offset : " + std::to_string(G::VoxLightSettings->specular_offset)); }

			else if (input[0] == "exit" || input[0] == "quit" || input[0] == "q") window->exit();

			else print(this, "Unknwon Command");
		}

		//Calling checkErrors or similar methods here will lead to automatic exceptions because it does not share the OpenGL context of the main thread.

		settingMutex.unlock();
	}
}

void Engine::toggle(bool* var, std::string name) {
	(*var) = !(*var);
	print(this, name + ((*var) ? ": ON" : ": OFF"));
}
