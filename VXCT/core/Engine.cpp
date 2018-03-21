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

Engine::Engine() {

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
	Window* window = new Window(); //create opengl context with GLFW

	loadGlad();	//Load Glad Extension library

	glEnable(GL_DEPTH_TEST); //turn on z-buffer
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	shaderInfo(); //Display shader info

	//==================================================================================

	//Shader lightingShader(COLORSHADER_VS, COLORSHADER_FS);
	Shader lampShader(EMITSHADER_VS, EMITSHADER_FS);
	Shader lightingShader(VOXSHADER_VS, VOXSHADER_FS, VOXSHADER_GS);
	//Shader lightingShader(VOXSHADER_VS, VOXSHADER_FS);

	//==================================================================================

	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vdata), Vdata, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Render loop:
	while (!window->shouldClose())
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		G::deltaTime = currentFrame - G::lastFrame;
		G::lastFrame = currentFrame;

		// input
		// -----
		//processInput(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// be sure to activate shader when setting uniforms/drawing objects
		//glViewport(0, 0, 100, 100);
		lightingShader.use();
		lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("lightPos", lightPos);
		lightingShader.setVec3("viewPos", G::SceneCamera->Position);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(G::SceneCamera->Zoom), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = G::SceneCamera->GetViewMatrix();
		lightingShader.setMat4("proj_u", projection);
		lightingShader.setMat4("view_u", view);

		// world transformation
		glm::mat4 model;
		lightingShader.setMat4("model_u", model);

		// render the cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// also draw the lamp object
		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		lampShader.setMat4("model", model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window->getGLFWwindow());
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate(); //Delete and clean all glfw resources allocated
	return;
}

void Voxelize() {
	Shader voxShader(VOXSHADER_VS, VOXSHADER_FS, VOXSHADER_GS);
	voxShader.use();
	//TODO
}